#pragma once

#include "Position.h"

enum Command: unsigned char{
	Move, Draw, LoopStart, LoopEnd, End
};

struct Instruction{
	public:
		Command command;
		Position pos;
};

class ActionQueue; //forward declaration to avoid #include

struct InstructionList{
	private:
        ActionQueue &actions;
		int point;
		int end;
		Position currentPos;
    public:
        //constructor (for generating a new ActionQueue from scrach)
        InstructionList(ActionQueue &actions_) : actions(actions_) 
        {
            point = 0;
            currentPos = create_position(0,0);
        }

		Instruction instructions[100];
        void read_data();
		void addInstruction(const Instruction &Instruction);
		void executeNext();
		void finalize();
};

void draw_line(ActionQueue &actions, Position, Position);

void goto_point(ActionQueue &actions, Position, unsigned short);

Command from_char(unsigned char);
