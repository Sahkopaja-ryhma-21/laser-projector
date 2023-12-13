#pragma once

#include "Position.h"

const int LIST_SIZE = 300;

enum Command: unsigned char{
	Move,Draw, LoopStart, LoopEnd, End
};

struct Instruction{
	public:
		Command command;
		Position pos;
};

struct InstructionList{
	public:
		Instruction instructions[LIST_SIZE];
		void addInstruction(Instruction Instruction);
		void executeNext();
		void finalize();
		void clear();
	private:
		int point = 0;
		Position currentPos = create_position(0,0);
};

Command from_char(unsigned char);
