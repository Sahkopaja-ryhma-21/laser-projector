#include "Command.h"
#include "Position.h"
#include "Motors.h"

void InstructionList::addInstruction(Instruction instruction){
	instructions[point] = instruction;
	point++;
}

void InstructionList::executeNext(){
	Instruction ins = instructions[point];
	switch (ins.command){
		case Command::Move:{
			goto_point(ins.pos);
			currentPos = ins.pos;
			point++;
			break;
		};
		case Command::Draw:{
			draw_line(ins.pos, currentPos);
			currentPos = ins.pos;
			point++;
			break;
		};
		case Command::End:{
			point = 0;
			break;
		};
		default:
			point = 0;
			break;
	}
}

void InstructionList::finalize(){
	end = point;
	point = 0;
}

Command from_char(unsigned char t){
	switch (t){
		case 1:
			return Command::Move;
			break;
		case 2:
			return Command::Draw;
			break;
		default:
			return Command::End;
			break;
	};
}
