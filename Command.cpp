#include "Command.h"
#include "Position.h"
#include "ActionQueue.h"

void InstructionList::read_data() {
	short i = 0;
	while (true){
		while (Serial.available() < 3);

		unsigned char comm = Serial.read();
		unsigned char posx = Serial.read();
		unsigned char posy = Serial.read();
 
		// full zeroes tell that the input is over and time to get to work.

		Instruction instruction;
		instruction.command = from_char(comm);
		instruction.pos = create_position(posx, posy);
		this->addInstruction(instruction);
		if ((comm | posy | posx) == 0){

			//digitalWrite(STATUS_LED, HIGH); //this interfered with debugging. TODO: re-enable
			this->finalize();
            return;
		}
		i += 3;
        
        if (i >= 1026)
        {
            Serial.println("calling the reset vector for some reason");
            delay(100);
            void(* resetFunc) (void) = 0;
            resetFunc();
        }
	}
}

void InstructionList::addInstruction(const Instruction &instruction){
	instructions[point] = instruction;
	point++;
}

void InstructionList::executeNext(){
	Instruction ins = instructions[point];
	switch (ins.command){
		case Command::Move:{
			goto_point(actions, ins.pos,currentPos.time_between(ins.pos));
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


/// Draws a line from current position to position p1
void draw_line(ActionQueue &actions, Position pos, Position last_pos){
    actions.pushLaserState(true);

	// Bresenham's algorhythm
	// https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm
	int dx = pos.x - last_pos.x;
	int dy = pos.y - last_pos.y;

	int dx1 = dx < 0 ? -dx : dx;
	int dy1 = dy < 0 ? -dy : dy;

	int px = (dy1*2)-dx1; 
	int py = (dx1*2)-dy1;

	int x = last_pos.x;
	int y = last_pos.y;

	int i = 1;

	// x-dominant
	if(dy1 <= dx1){

		// right to left
		if(dx<0)
			i = -1;

		Position goto_pos;
		goto_pos.x = x;
		goto_pos.y = y;
		goto_point(actions, goto_pos, GOTO_WAIT_TIME);

		// Rasterize the line
		for (; x != pos.x; x+=i) {
			// Deal with octants...
			if (px < 0) {
				px += 2 * dy1;
			} else {
				if (dy >= 0) {
				    y++;
				} else {
				    y--;
				}
				px += 2 * (dy1 - dx1);
			}
			// Draw pixel from line span at
		    // currently rasterized position
			goto_pos.x = x;
			goto_pos.y = y;
			goto_point(actions, goto_pos, GOTO_WAIT_TIME);
		}

	} else { // The line is Y-axis dominant        

		// line is drawn bottom to top
		if(dy<0)
			i = -1;
			
		Position goto_pos;
		goto_pos.x = x;
		goto_pos.y = y;
		goto_point(actions, goto_pos, GOTO_WAIT_TIME);
			
		// Rasterize the line
		for (; y != pos.y; y+=i) {
			// Deal with octants...
			if (py <= 0) {
				py += 2 * dx1;
			} else {
				if (dx >= 0) {
					x++;
				} else {
					x--;
				}
			py += 2 * (dx1 - dy1);
		    }            
			// Draw pixel from line span at
		    // currently rasterized position
		goto_pos.x = x;
		goto_pos.y = y;
		goto_point(actions, goto_pos, GOTO_WAIT_TIME);
		}
	}

    actions.pushLaserState(false);
}

void goto_point(ActionQueue &actions, Position pos, unsigned short time) {
    actions.pushMotorPos(Recipient::X, pos.x);
    actions.pushMotorPos(Recipient::Y, pos.y);
    actions.pushSetInterval(2*time);
	actions.pushWaitForNextpopAndExecute();
}
