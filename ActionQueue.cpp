#include "ActionQueue.h"

#include <SPI.h>
#include <stdint.h>
#include <avr/io.h>
#include "Constants.h"

void ActionQueue::begin()
{
    SPI.begin();
    SPI.beginTransaction(SPISettings(2000000, MSBFIRST, SPI_MODE0));
    pinMode(CSY, OUTPUT);
    pinMode(LASER_PIN, OUTPUT);
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

void ActionQueue::pushSpiPacket(Recipient recipient, uint8_t *packet, uint8_t packetSize = 1)
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

void ActionQueue::pushLaserState(bool laserOn)
{
    action[newestIndex] = Action::SET_LASER;
    data[newestIndex] = laserOn;
    addNewest();
}

//Push a wait action on the queue. When executeCommands() encounters this action, it stops
//..executing queued commands, and only resumes when called again (by the timed interrupt)
void ActionQueue::pushUpdateEnd()
{
    action[newestIndex] = Action::WAIT_DELAY;
    addNewest();
}

//Configure the interval between timed interrupts. The duration is in units of microseconds, and
//..has a maximum delay of 2550 (2,55 milliseconds). The resolution is 10 us (to fit in a byte).
//..The preceding action on the queue should be a pushWait() call, to ensure that the hardware
//..timer is reconfigured in time before the next interrupt is due. 
void ActionQueue::pushSetInterval(uint16_t delay_us)
{
    /*action[newestIndex] = Action::SET_DELAY;
    data[newestIndex] = delay_us / 10;
    addNewest();*/
}

void ActionQueue::update()
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