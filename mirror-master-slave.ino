#include "Position.h"
#include "Command.h"
#include "Motors.h"
#include <SPI.h>
#include "Serial.h"
#include "Constants.h"

const InstructionList commands;
void(* resetFunc) (void) = 0;

void setup() {
	Serial.begin(9600);
	SPI.begin();
	SPI.beginTransaction(SPISettings(2000000, MSBFIRST, SPI_MODE0));
	pinMode(CSY, OUTPUT);
	pinMode(CSX, OUTPUT);
	pinMode(LASER_PIN, OUTPUT);
	pinMode(STATUS_LED, OUTPUT);
	digitalWrite(CSX, HIGH);
	digitalWrite(CSY, HIGH);
	commands = read_data();
	enable_motor(CSX);
	enable_motor(CSY);
	long start_time = millis();

	while(millis() - start_time < 1000){
		useSerial();
	}
}

void loop() {
	commands.executeNext();
}

InstructionList read_data() {
	InstructionList list;
	short i = 0;
	while (true){
	while (Serial.available()<3){}

	unsigned char comm = Serial.read();
	unsigned char posx = Serial.read();
	unsigned char posy = Serial.read();

	// full zeroes tell that the input is over and time to get to work.

	Instruction instruction;
	instruction.command = from_char(comm);
	instruction.pos = create_position(posx, posy);
	list.addInstruction(instruction);
	if ((comm | posy | posx)  == 0){
		digitalWrite(STATUS_LED, HIGH);
		list.finalize();
		return list;
	}
	i+=3;
	if (i>=1026) resetFunc();
	}
}
