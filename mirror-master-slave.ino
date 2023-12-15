#include "Position.h"
#include "Command.h"
#include <SPI.h>
#include "Serial.h"
#include "Constants.h"
#include <stdint.h>
#include "ActionQueue.h"
#include "Serial.h"
#include <avr/interrupt.h>

ActionQueue actions;
InstructionList commands(actions);

//timer 1 overflow interrupt
//timer1 is confiugred to generate these by actions.begin()
ISR(TIMER1_OVF_vect)
{
    digitalWrite(5, HIGH);
    actions.popAndExecute();
    digitalWrite(5, LOW);
}

void setup() {
	Serial.begin(115200);
	Serial.println("reset");
	pinMode(STATUS_LED, OUTPUT);
	pinMode(5, OUTPUT);
	pinMode(6, OUTPUT);
	pinMode(3, OUTPUT);
	pinMode(2, OUTPUT);
	actions.begin();
	actions.pushMotorDisable(Recipient::XY);
	Serial.println("entering \"commands.read_data()\"");
	commands.read_data();
	Serial.println("exiting \"commands.read_data()\"");
	actions.pushSetDelayPeriod(1000); //set the delay to 1 millisecond
	actions.pushMotorEnable(Recipient::XY, ConfigSlot::CONFIG_DEFAULT);
	for(int n = 0; n < 500; ++n)
        actions.pushDelay(); //wait 500 ms for the servo to initialize
	actions.pushDelay();
	actions.pushMotorCenterPos(Recipient::X, 80);
	actions.pushDelay();
	actions.pushMotorCurrentGain(Recipient::XY,  7,  16,  2);
	actions.pushDelay();
	actions.pushMotorVelocityGain(Recipient::XY, 30,  0, 15);
	actions.pushDelay();
	actions.pushMotorAngleGain(Recipient::XY,  8,  0, 20);
	actions.pushDelay();
	actions.pushMotorScale(Recipient::Y, 200);
	actions.pushDelay();
    actions.pushMotorStoreConfig(Recipient::XY, ConfigSlot::CONFIG_SLOT_1);
    actions.pushDelay();

	Serial.println("ending \"setup()\"");
}

void loop() {
	commands.executeNext();
	useSerial(&commands, actions);
}
