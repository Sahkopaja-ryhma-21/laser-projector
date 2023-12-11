#ifndef ACTIONQUEUE_H
#define ACTIONQUEUE_H

#include <SPI.h>
#include <stdint.h>
#include <avr/io.h>
#include "Interpolation.h"

enum class Recipient : uint8_t {X, Y};

//First in, first out action buffer.
struct ActionQueue
{
    enum class Action : uint8_t {
            X_START_PACKET, Y_START_PACKET, //Clear chip select and send one byte of data
            PACKET_DATA,                    //Do not touch chip select, just send data.
            END_PACKET,                     //Do not send data, just set chip select
            SET_LASER,                      //Change the state of the laser output
            SET_DELAY,                      //Reschedule the next timer interrupt
            WAIT_DELAY                      //Stop executing actions and wait for the next timer interrupt
        };

    static constexpr uint8_t SIZE = 64;
    static constexpr uint8_t INDEX_MASK = SIZE - 1;

    volatile Action  action[SIZE];          //The action to be taken
    volatile uint8_t data[SIZE];            //Data related to the action
    volatile uint8_t newestIndex = 0;       //Index to the newest byte in the buffer
    volatile uint8_t oldestIndex = 0;       //Index of the oldest byte in the queue (next one to be sent)
    volatile uint8_t activeChipSelect = 0;  //Pin number of currently asserted chip select output

    void begin()
    {
        SPI.begin();
        SPI.beginTransaction(SPISettings(2000000, MSBFIRST, SPI_MODE0));
        pinMode(CSY, OUTPUT);
        pinMode(CSX, OUTPUT);
        
        digitalWrite(CSX, HIGH);
        digitalWrite(CSY, HIGH);

        //Timer1 configuration. Timer1 is used to trigger periodic SPI and laser updates.
        //In this timer mode, the timer value increments once every clock cycle (at 16 MHz) until the value
        //..reaches ICR1. Then the timer generates an overflow interrupt and restarts from 0.
        TCNT1  = 0; //clear the timer count
        ICR1   = 16000000UL / 10000; //10 kHz frequency
        TCCR1A = (1<<WGM11)  | (0<<WGM10); //fast pwm mode, TOP = ICR1
        TCCR1B = (1<<WGM13)  | (1<<WGM12); 
        TIMSK1 = (1<<TOIE1); //timer 1 overflow interrupt enabled
        TCCR1B |= (1<<CS10); //start the timer with a prescaler of 1/1
    }

    uint8_t getLength()
    {
        return (newestIndex - oldestIndex) & INDEX_MASK;
    }

    uint8_t getFreeCapacity()
    {
        return (SIZE - 1) - getLength();
    }

    bool isEmpty()
    {
        return newestIndex == oldestIndex;
    }

    bool isFull()
    {
        return (SIZE - 1) == getLength();
    }

    constexpr uint8_t getCapacity()
    {
        return SIZE;
    }

    inline void addNewest()
    {
        newestIndex = (newestIndex + 1) & INDEX_MASK;
    }

    inline void removeOldest()
    {
        oldestIndex = (oldestIndex + 1) & INDEX_MASK;
    }

    //not bounds checked!
    void pushSpiPacket(Recipient recipient, uint8_t *packet, uint8_t packetSize = 1)
    {
        if(recipient == Recipient::X)
            action[newestIndex] = Action::X_START_PACKET;
        else
            action[newestIndex] = Action::Y_START_PACKET;
        
        for(uint8_t i = 0; i < packetSize; ++i)
        {
            data[newestIndex] = packet[i];
            addNewest();
            action[newestIndex] = Action::PACKET_DATA;
        }
        action[newestIndex] = Action::END_PACKET;
        addNewest();
    }

    void pushLaserState(bool laserOn)
    {
        action[newestIndex] = Action::SET_LASER;
        data[newestIndex] = laserOn;
        addNewest();
    }

    //Push a wait action on the queue. When executeCommands() encounters this action, it stops
    //..executing queued commands, and only resumes when called again (by the timed interrupt)
    void pushWait()
    {
        action[newestIndex] = Action::WAIT_DELAY;
        addNewest();
    }

    //Configure the interval between timed interrupts. The duration is in units of microseconds, and
    //..has a maximum delay of 2550 (2,55 milliseconds). The resolution is 10 us (to fit in a byte).
    //..The preceding action on the queue should be a pushWait() call, to ensure that the hardware
    //..timer is reconfigured in time before the next interrupt is due. 
    void pushSetInterval(uint16_t delay_us)
    {
        action[newestIndex] = Action::SET_DELAY;
        data[newestIndex] = delay_us / 10;
        addNewest();
    }

    void executeCommands()
    {
        while(!this->isEmpty())
        {
            switch(action[oldestIndex])
            {
            case Action::X_START_PACKET:
                activeChipSelect = CSX;
                digitalWrite(activeChipSelect, LOW);
                SPI.transfer(data[oldestIndex]);
                break;

            case Action::Y_START_PACKET:
                activeChipSelect = CSY;
                digitalWrite(activeChipSelect, LOW);
                SPI.transfer(data[oldestIndex]);
                break;

            case Action::PACKET_DATA:
                SPI.transfer(data[oldestIndex]);
                break;

            case Action::END_PACKET:
                digitalWrite(activeChipSelect, HIGH);
                break;

            case Action::SET_LASER:
                digitalWrite(LASER_PIN, data[oldestIndex]);
                break;

            case Action::SET_DELAY:
                ICR1 = F_CPU / 100000 * data[oldestIndex];
                break;

            default:
            case Action::WAIT_DELAY:
                removeOldest();
                return;
            }
            removeOldest();
        }
    }
};

#endif//ACTIONQUEUE_H
