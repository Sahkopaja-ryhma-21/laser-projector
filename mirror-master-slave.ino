#include "Position.h"
#include <EEPROM.h>
#include "Command.h"
#include "Motors.h"
#include <SPI.h>

const InstructionList commands;

void setup() {
	Serial.begin(9600);
	SPI.begin();
	SPI.beginTransaction(SPISettings(14000, MSBFIRST, SPI_MODE0));
	pinMode(CSY, OUTPUT);
	pinMode(CSX, OUTPUT);
	pinMode(LASER_PIN, OUTPUT);
	pinMode(STATUS_LED, OUTPUT);
	commands = read_data();
}

void loop() {
	int i = 0;
	while (true){
		commands.executeNext();
	}
}

InstructionList read_data() {
	InstructionList list;
	short i = 0;
	while (true){
		while (Serial.available()<3){
		}

		char comm = Serial.read();
		char posx = Serial.read();
		char posy = Serial.read();

		// full zeroes tell that the input is over and time to get to work.
		if ((comm | posy | posx)  == 0){
			digitalWrite(STATUS_LED, HIGH);
			return list;
		}
		Instruction instruction;
		instruction.command = from_char(comm);
		instruction.pos = create_position(posx, posy);
		list.addInstruction(instruction);

		i+=3;
		if (i>=1026) return list;
	}
}
