#include "Position.h"
#include "Command.h"
#include "Motors.h"
#include <SPI.h>
#include "Serial.h"
#include "Constants.h"

const InstructionList commands;

void setup() {
	Serial.begin(9600);
	pinMode(CSY, OUTPUT);
	pinMode(CSX, OUTPUT);
	pinMode(LASER_PIN, OUTPUT);
	pinMode(STATUS_LED, OUTPUT);
	digitalWrite(CSX, HIGH);
	digitalWrite(CSY, HIGH);
	SPI.begin();
	SPI.beginTransaction(SPISettings(2000000, MSBFIRST, SPI_MODE0));
	flip_motor(CSY);
	commands.clear();
}

void loop() {
	commands.executeNext();
	useSerial(&commands);
}
