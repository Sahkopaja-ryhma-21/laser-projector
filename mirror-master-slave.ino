#include "Position.h"
#include "Command.h"
#include <SPI.h>
#include "Serial.h"
#include "Constants.h"
#include <stdint.h>
#include "ActionQueue.h"
#include "Serial.h"
#include <avr/interrupt.h>

InstructionList commands;
ActionQueue actions;

//timer 1 overflow interrupt
//timer1 is confiugred to generate these by actions.begin()
ISR(TIMER1_OVF_vect)
{
    actions.update();
}

void(* resetFunc) (void) = 0;

void setup() {
	Serial.begin(9600);
	pinMode(STATUS_LED, OUTPUT);
    actions.begin();
    actions.pushMotorDisable(Recipient::X);
    actions.pushMotorDisable(Recipient::Y);
	commands = read_data();
    actions.pushMotorEnable(Recipient::X, ConfigSlot::CONFIG_DEFAULT);
    actions.pushMotorEnable(Recipient::Y, ConfigSlot::CONFIG_DEFAULT);
    actions.pushMotorReverse(Recipient::Y);
	long start_time = millis();

	while(millis() - start_time < 1000){
		useSerial(actions);
	}
}

void loop() {
	commands.executeNext();
	useSerial(actions);
}

InstructionList read_data() {
	InstructionList list;
	short i = 0;
	while (true){
		while (Serial.available() < 3);

		unsigned char comm = Serial.read();
		unsigned char posx = Serial.read();
		unsigned char posy = Serial.read();
 
		// full zeroes tell that the input is over and time to get to work.

		Instruction instruction;
		instruction.command = from_char(comm);
		instruction.pos = create_position(posx, posy);
		list.addInstruction(instruction);
		if ((comm | posy | posx) == 0){
			digitalWrite(STATUS_LED, HIGH);
			list.finalize();
			return list;
		}
		i += 3;
		if (i >= 1026) resetFunc();
	}
}
