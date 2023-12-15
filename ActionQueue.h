#ifndef ACTIONQUEUE_H
#define ACTIONQUEUE_H

#include <SPI.h>
#include <stdint.h>
#include <avr/io.h>
#include "Constants.h"
#include "MotorSpiCommands.h"
#include <util/atomic.h>

enum class Recipient : uint8_t {X, Y, XY};

//First in, first out action buffer.
class ActionQueue
{
private:
    enum class Action : uint8_t {
            X_START_PACKET, Y_START_PACKET, //Clear chip select and send one byte of data
            PACKET_DATA,                    //Do not touch chip select, just send data.
            END_PACKET,                     //Do not send data, just set chip select
            SET_LASER,                      //Change the state of the laser output
            SET_DELAY,                      //Reschedule the next timer interrupt
            SET_LONG_DELAY_H,
            SET_LONG_DELAY_L,
            WAIT_DELAY                      //Stop executing actions and wait for the next timer interrupt
        };

    static constexpr uint8_t SIZE = 32;
    static constexpr uint8_t INDEX_MASK = SIZE - 1;

    volatile uint8_t newestIndex = 0;       //Index to the newest byte in the buffer
    volatile uint8_t oldestIndex = 0;       //Index of the oldest byte in the queue (next one to be sent)
    volatile Action  action[SIZE];          //The action to be taken
    volatile uint8_t data[SIZE];            //Data related to the action
    volatile uint8_t activeChipSelect = 0;  //Pin number of currently asserted chip select output
    volatile uint8_t delayTempH = 0;         //Temporary storage for the high byte of the next delay duration 

    inline void push()
    {
        PORTD |= (1u << PORTD7); //STATUS_LED on to indicate a full action buffer
        while(isFull());
        newestIndex = (newestIndex + 1) & INDEX_MASK;
        PORTD &= ~(1u << PORTD7); //STATUS_LED off
    }

    inline void pop()
    {
        oldestIndex = (oldestIndex + 1) & INDEX_MASK;
    }

public:
    inline uint8_t getLength()
    {
        return (newestIndex - oldestIndex) & INDEX_MASK;
    }

    constexpr uint8_t getCapacity()
    {
        return SIZE - 1;
    }

    inline uint8_t getFreeCapacity()
    {
        return getCapacity() - getLength();
    }

    inline bool isEmpty()
    {
        return newestIndex == oldestIndex;
    }

    inline bool isFull()
    {
        return getCapacity() == getLength();
    }

    void begin();

    void pushSpiPacket(Recipient recipient, uint8_t *packet, uint8_t packetSize = 1);

    //Load a configuration and set the initialization state. By default, loads the default conf. 
    //..and starts the servo.
    void pushMotorEnable(
            Recipient axis, ConfigSlot slot = ConfigSlot::CONFIG_DEFAULT
        )
    {
        uint8_t spiPacket[] = {Parameter::PARAM_LOAD_CONFIG, slot, InitState::INIT_ANGLE_CTRL};
        pushSpiPacket(axis, spiPacket, sizeof(spiPacket));
    }

    //Shuts down a servo
    void pushMotorDisable(Recipient axis)
    {
        uint8_t spiPacket[] = {Parameter::PARAM_INIT_STATE, InitState::INIT_OFF};
        pushSpiPacket(axis, spiPacket, sizeof(spiPacket));
    }

    //Shuts down a servo
    void pushMotorStoreConfig(Recipient axis, ConfigSlot slot = ConfigSlot::CONFIG_DEFAULT)
    {
        uint8_t spiPacket[] = {Parameter::PARAM_STORE_CONFIG, slot};
        pushSpiPacket(axis, spiPacket, sizeof(spiPacket));
    }

    inline void pushMotorCurrentGain(Recipient axis, uint8_t p, uint8_t i, uint8_t d)
    {
        uint8_t spiPacket[] = {Parameter::PARAM_CURRENT_GAIN_P, p, i, d};
        pushSpiPacket(axis, spiPacket, sizeof(spiPacket));
    }

    inline void pushMotorVelocityGain(Recipient axis, uint8_t p, uint8_t i, uint8_t d)
    {
        uint8_t spiPacket[] = {Parameter::PARAM_VELOCITY_GAIN_P, p, i, d};
        pushSpiPacket(axis, spiPacket, sizeof(spiPacket));
    }

    inline void pushMotorAngleGain(Recipient axis, uint8_t p, uint8_t i, uint8_t d)
    {
        uint8_t spiPacket[] = {Parameter::PARAM_ANGLE_GAIN_P, p, i, d};
        pushSpiPacket(axis, spiPacket, sizeof(spiPacket));
    }

    inline void pushMotorReverse(Recipient axis, bool flipped = true)
    {
        uint8_t spiPacket[] = {Parameter::PARAM_INVERTED, (uint8_t)(flipped ? 1 : 0)};
        pushSpiPacket(axis, spiPacket, sizeof(spiPacket));
    }

    inline void pushMotorScale(Recipient axis, uint8_t factor = 128) //255 uses the maximum possible output range
    {
        uint8_t spiPacket[] = {Parameter::PARAM_ANGLE_SCALE, factor};
        pushSpiPacket(axis, spiPacket, sizeof(spiPacket));
    }

    inline void pushMotorCenterPos(Recipient axis, uint8_t offset = 128) //0 moves the center position to one extreme, 255 to the other
    {
        uint8_t spiPacket[] = {Parameter::PARAM_ANGLE_CENTER, offset};
        pushSpiPacket(axis, spiPacket, sizeof(spiPacket));
    }

    inline void pushMotorPos(Recipient axis, uint8_t position)
    {
        pushSpiPacket(axis, &position, 1);
    }

    void pushLaserState(bool laserOn);

    //Push a wait action on the queue. When executeCommands() encounters this action, it stops
    //..executing queued commands, and only resumes when called again (by the timed interrupt)
    void pushDelay(uint16_t periods = 1);

    //Configure the interval between timed interrupts. The duration is in units of microseconds, and
    //..has a maximum delay of 2550 (2,55 milliseconds). The resolution is 10 us (to fit in a byte).
    //..The preceding action on the queue should be a pushWait() call, to ensure that the hardware
    //..timer is reconfigured in time before the next interrupt is due. 
    void pushSetDelayPeriod(uint16_t microseconds);

    void popAndExecute();
};

#endif//ACTIONQUEUE_H
