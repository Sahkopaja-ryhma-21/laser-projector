#pragma once

#include "Position.h"

enum Command: unsigned char{
	Move,Draw, LoopStart, LoopEnd, End
};

struct Instruction{
	public:
		Command command;
		Position pos;
};

class ActionQueue; //forward declaration to avoid #include

struct InstructionList{
	public:
		Instruction instructions[200];
		void addInstruction(Instruction Instruction);
		void executeNext();
		void finalize();
	private:
		int point = 0;
		int end;
        ActionQueue *actions;
		Position currentPos = create_position(0,0);
};

void draw_line(ActionQueue &actions, Position, Position);

void goto_point(ActionQueue &actions, Position, unsigned short);

Command from_char(unsigned char);
