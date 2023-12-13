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
	Serial.begin(9600);
    Serial.println("reset");
	pinMode(STATUS_LED, OUTPUT);
	pinMode(5, OUTPUT);
	pinMode(6, OUTPUT);
    pinMode(3, OUTPUT);
    pinMode(2, OUTPUT);
    actions.begin();
    actions.pushMotorDisable(Recipient::X);
    actions.pushMotorDisable(Recipient::Y);
    Serial.println("entering \"commands.read_data()\"");
	commands.read_data();
    Serial.println("exiting \"commands.read_data()\"");
    actions.pushMotorEnable(Recipient::X, ConfigSlot::CONFIG_DEFAULT);
    actions.pushMotorEnable(Recipient::Y, ConfigSlot::CONFIG_DEFAULT);
    actions.pushMotorReverse(Recipient::Y);
	long start_time = millis();
    Serial.println("ending \"setup()\"");
	//while(millis() - start_time < 1000){
	//	useSerial(actions);
	//}
}

void loop() {
	commands.executeNext();
	useSerial(actions);
}
