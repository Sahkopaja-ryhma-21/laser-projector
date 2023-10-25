#include "Command.h"
#include "Position.h"

void InstructionList::addInstruction(Instruction instruction){
	instructions[point] = instruction;
	point++;
}

Command from_char(char t){
	switch (t){
		case 1:
			return Command::Line;
			break;
		default:
			return Command::End;
			break;
	};
}
