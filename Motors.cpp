#include "Motors.h"
#include "Arduino.h"
#include <SPI.h>

void move_motor(unsigned char pos,unsigned char pin){
	digitalWrite(pin, LOW);
	SPI.transfer(pos);
	digitalWrite(pin, HIGH);
}

/// Draws a line from current position to position p1
void draw_line(Position pos, Position last_pos){
	digitalWrite(LASER_PIN, HIGH);

	// Bresenham's algorhythm
	// https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm
	int dx = pos.x - last_pos.x;
	int dy = pos.y - last_pos.y;
	int yi = 1;

	if(dy<0){
		dy = -dy;
		yi = -1;
	}
	
	int d = (dy>>2)-dx;
	int y = last_pos.y;

	for(x = last_pos.x; x<pos.x; x++){
		Position goto_pos;
		goto_pos.x = x;
		goto_pos.y = y;
		goto_point(goto_pos);

		if(dy>0){
			y += yi;
			D += (dy-dx)>>2;

		} else {
			D += dy>>2;
		}
	}

	digitalWrite(LASER_PIN, LOW);
}

void goto_point(Position pos) {
	move_motor(pos.x,CSX);
	move_motor(pos.y,CSY);
}
