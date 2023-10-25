#include <SPI.h>
#include "Position.h"
#include <EEPROM.h>
#include "Command.h"

#define CSX 10
#define CSY 9
#define t 10
#define WAIT_TIME 10
#define LASER_PIN 4

void setup() {
	Serial.begin(9600);
	SPI.begin();
	SPI.beginTransaction(SPISettings(14000, MSBFIRST, SPI_MODE0));
	pinMode(CSY, OUTPUT);
	pinMode(CSX, OUTPUT);
	pinMode(LASER_PIN, OUTPUT);
	InstructionList commands = read_data();
}

void loop() {
}

void move_motor(byte pos, byte pin){
	digitalWrite(pin, LOW);
	SPI.transfer(pos);
	digitalWrite(pin, HIGH);
}

void draw_line(Position p1, Position p2){
	digitalWrite(LASER_PIN, LOW);
	goto_point(p1);
	digitalWrite(LASER_PIN, HIGH);
	goto_point(p2);
	// FIX: Master must do interpolation.
	delay(p1.distance(p2));
	digitalWrite(LASER_PIN, LOW);
}

void goto_point(Position pos) {
	move_motor(pos.x,CSX);
	move_motor(pos.y,CSY);
}

InstructionList read_data() {
	InstructionList list;
	Serial.println("Please input your image");
	short i = 0;
	while (true){
		while (Serial.available()<3){
		}

		char comm = Serial.read();
		Serial.print("Recieved");
		Serial.println(comm);
		char posx = Serial.read();
		Serial.print("Recieved");
		Serial.println(posx);
		char posy = Serial.read();
		Serial.print("Recieved");
		Serial.println(posy);


		// full zeroes tell that the input is over and time to get to work.
		if ((comm | posy | posx)  == 0){
			Serial.println("Input ended. Beginning drawing");
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
