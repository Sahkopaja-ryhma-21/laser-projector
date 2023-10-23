#pragma once
#include "Position.h"

enum Command{Line, LoopStart, LoopEnd};

struct Instruction{
	public:
		Command command;
		Position pos1;
		Position pos2;
		Instruction(char command,Position p1,Position p2);
};

struct InstructionList{
	public:
		Instruction instructions[1026];
		void addInstruction(Instruction Instruction);

	private:
		int point = 0;
};
