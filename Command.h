#pragma once
#include "Position.h"

enum Command{Line, LoopStart, LoopEnd, End};

struct Instruction{
	public:
		Command command;
		Position pos;
};

struct InstructionList{
	public:
		Instruction instructions[1026];
		void addInstruction(Instruction Instruction);
	private:
		int point = 0;
};

Command from_char(char);
