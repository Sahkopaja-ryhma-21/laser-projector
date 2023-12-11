#include "Position.h"
#include "ActionQueue.h"

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
		Instruction instructions[501];
		void addInstruction(Instruction Instruction);
		void executeNext(ActionQueue &actions);
		void finalize();
	private:
		int point = 0;
		int end;
		Position currentPos = create_position(0,0);
};

Command from_char(unsigned char);
