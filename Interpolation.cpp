#include "ActionQueue.h"
#include "Constants.h"
#include "Interpolation.h"
#include "Arduino.h"

/// Draws a line from current position to position p1
void draw_line(ActionQueue &actions, Position pos, Position last_pos){
    while(actions.getLength() > actions.getCapacity() / 2);
    actions.pushSetInterval(GOTO_WAIT_TIME);
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

		//Position goto_pos;
		//goto_pos.x = x;
		//goto_pos.y = y;
		//goto_point(goto_pos, GOTO_WAIT_TIME);
        while(actions.getLength() > actions.getCapacity() / 2);
        actions.pushSpiPacket(Recipient::X, x);
        actions.pushSpiPacket(Recipient::Y, y);
        actions.pushWait();

		// Rasterize the line
		for (x; x < pos.x || x > pos.x; x+=i) {  //Mikä ajatus tässä on? Pelkkä "for(; x != pos.x; x+=i) {" ajaisi saman asian.
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
			//goto_pos.x = x;
			//goto_pos.y = y;
			//goto_point(goto_pos, GOTO_WAIT_TIME);
            while(actions.getLength() > actions.getCapacity() / 2);
            actions.pushSpiPacket(Recipient::X, x);
            actions.pushSpiPacket(Recipient::Y, y);
            actions.pushWait();
		}

	} else { // The line is Y-axis dominant        

		// line is drawn bottom to top
		if(dy<0)
			i = -1;
			
		//Position goto_pos;
		//goto_pos.x = x;
		//goto_pos.y = y;
		//goto_point(goto_pos, GOTO_WAIT_TIME);
        while(actions.getLength() > actions.getCapacity() / 2);
        actions.pushSpiPacket(Recipient::X, x);
        actions.pushSpiPacket(Recipient::Y, y);
        actions.pushWait();
			
		// Rasterize the line
		for (y; y < pos.y || y > pos.y; y+=i) { //Mikä ajatus tässä on? Pelkkä "for(; y != pos.y; y+=i) {" ajaisi saman asian.
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
		//goto_pos.x = x;
		//goto_pos.y = y;
		//goto_point(goto_pos, GOTO_WAIT_TIME);
        while(actions.getLength() > actions.getCapacity() / 2);
        actions.pushSpiPacket(Recipient::X, x);
        actions.pushSpiPacket(Recipient::Y, y);
        actions.pushWait();
		}
	}

	actions.pushLaserState(false);
}
