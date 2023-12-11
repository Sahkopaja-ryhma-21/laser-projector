#include "Position.h"
#include "Command.h"
#include "Interpolation.h"
#include "ActionQueue.h"
#include <SPI.h>

ActionQueue actions;
const InstructionList commands;
void(* resetFunc) (void) = 0;

ISR(TIMER1_OVF_vect)
{
    actions.executeCommands();
}

void setup() {
    sei(); //enable interrupts
    pinMode(STATUS_LED, OUTPUT);
    actions.begin();
    uint8_t resetSequence[] = {30, 1, 5};
	actions.pushSpiPacket(Recipient::X, resetSequence, sizeof(resetSequence));
    actions.pushSpiPacket(Recipient::Y, resetSequence, sizeof(resetSequence));
	Serial.begin(9600);
	commands = read_data();

	delay(2000);
}

void loop() {
    //avoid overflowing the action buffer
    while(actions.getLength() > actions.getCapacity() / 2);
	commands.executeNext(actions);
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
