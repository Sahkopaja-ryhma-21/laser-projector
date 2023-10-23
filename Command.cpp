#include "Command.h"
#include "Position.h"

Instruction::Instruction(char command,Position p1,Position p2){
	Instruction(Command::Line, p1, p2);
}

void InstructionList::addInstruction(Instruction instruction){
	this->instructions[point] = instruction;
	this->point++;
}
