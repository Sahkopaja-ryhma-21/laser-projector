#ifndef ACTIONQUEUE_H
#define ACTIONQUEUE_H

#include <SPI.h>
#include <stdint.h>
#include <avr/io.h>
#include "Constants.h"
#include "MotorSpiCommands.h"

enum class Recipient : uint8_t {X, Y};

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
            WAIT_DELAY                      //Stop executing actions and wait for the next timer interrupt
        };

    static constexpr uint8_t SIZE = 64;
    static constexpr uint8_t INDEX_MASK = SIZE - 1;

    volatile Action  action[SIZE];          //The action to be taken
    volatile uint8_t data[SIZE];            //Data related to the action
    volatile uint8_t newestIndex = 0;       //Index to the newest byte in the buffer
    volatile uint8_t oldestIndex = 0;       //Index of the oldest byte in the queue (next one to be sent)
    volatile uint8_t activeChipSelect = 0;  //Pin number of currently asserted chip select output

    inline void addNewest()
    {
        newestIndex = (newestIndex + 1) & INDEX_MASK;
    }

    inline void removeOldest()
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
        return (SIZE - 1) - getLength();
    }

    inline bool isEmpty()
    {
        return newestIndex == oldestIndex;
    }

    inline bool isFull()
    {
        return (SIZE - 1) == getLength();
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

    inline void pushMotorReverse(Recipient axis, bool flipped = true)
    {
        uint8_t spiPacket[] = {Parameter::PARAM_INVERTED, flipped ? 1 : 0};
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
    void pushUpdateEnd();

    //Configure the interval between timed interrupts. The duration is in units of microseconds, and
    //..has a maximum delay of 2550 (2,55 milliseconds). The resolution is 10 us (to fit in a byte).
    //..The preceding action on the queue should be a pushWait() call, to ensure that the hardware
    //..timer is reconfigured in time before the next interrupt is due. 
    void pushSetInterval(uint16_t delay_us);

    void update();
};

#endif//ACTIONQUEUE_H
