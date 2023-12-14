#pragma once

#include "Position.h"

const int LIST_SIZE = 300;

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
		Instruction instructions[LIST_SIZE];
        	void read_data();
		void addInstruction(const Instruction &Instruction);
		void executeNext();
		void finalize();
		void clear();
};

void draw_line(ActionQueue &actions, Position, Position);

void goto_point(ActionQueue &actions, Position, unsigned short);

Command from_char(unsigned char);
