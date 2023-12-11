#include "Command.h"
#include "Position.h"
#include "Interpolation.h"

void InstructionList::addInstruction(Instruction instruction){
	instructions[point] = instruction;
	point++;
}

void InstructionList::executeNext(ActionQueue &actions){
	Instruction ins = instructions[point];
	switch (ins.command){
		case Command::Move:{
            actions.pushSetInterval(currentPos.time_between(ins.pos));
            actions.pushSpiPacket(Recipient::X, ins.pos.x);
            actions.pushSpiPacket(Recipient::Y, ins.pos.y);
			actions.pushWait();
			currentPos = ins.pos;
			point++;
			break;
		};
		case Command::Draw:{
			draw_line(actions, ins.pos, currentPos);
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
