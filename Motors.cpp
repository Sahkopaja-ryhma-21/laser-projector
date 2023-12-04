#include "Motors.h"
#include "Arduino.h"
#include <SPI.h>

void move_motor(unsigned char pos,unsigned char pin){
	digitalWrite(pin, LOW);
	SPI.transfer(pos);
	digitalWrite(pin, HIGH);
	delayMicroseconds(10);
}

/// Draws a line from current position to position p1
void draw_line(Position pos, Position last_pos){
	digitalWrite(LASER_PIN, HIGH);

	// Bresenham's algorhythm
	// https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm
	int dx = pos.x - last_pos.x;
	int dy = pos.y - last_pos.y;

	int dx1 = dx < 0 ? -dx : dx;
	int dy1 = dy < 0 ? -dy : dy;

	int px = (dy1*2)-dx1; 
	int py = (dx1*2)-dy1;

	int x;
	int y;
	int x2;
	int y2;
	int xe;
	int ye;

	// x-dominant
	if(dy1 <= dx1){

		// left to right
		if(dx>=0){
			x = last_pos.x;
			y = last_pos.y;
			x2 = pos.x;
		} else { // right to left
			x = pos.x;
			y = pos.y;
			x2 = last_pos.x;
		}
		Position goto_pos;
		goto_pos.x = x;
		goto_pos.y = y;
		goto_point(goto_pos);

		// Rasterize the line
		for (int i = 0; x < xe; i++) {
			x++;            // Deal with octants...
			if (px < 0) {
				px = px + 2 * dy1;
			} else {
				if ((dx < 0 && dy < 0) || (dx > 0 && dy > 0)) {
				    y = y + 1;
				} else {
				    y = y - 1;
				}
				px = px + 2 * (dy1 - dx1);
			}
				// Draw pixel from line span at
		    // currently rasterized position
			goto_pos.x = x;
			goto_pos.y = y;
			goto_point(goto_pos);
		}

	} else { // The line is Y-axis dominant        

			// Line is drawn bottom to top
		if (dy >= 0) {
			x = last_pos.x;
			y = last_pos.y;
			ye = pos.y;
		} else { // Line is drawn top to bottom
			x = pos.x;
			y = pos.y;
			ye = last_pos.y;
		}
			
		Position goto_pos;
		goto_pos.x = x;
		goto_pos.y = y;
		goto_point(goto_pos);
			
		// Rasterize the line
		for (int i = 0; y < ye; i++) {
			y = y + 1;            // Deal with octants...
			if (py <= 0) {
				py = py + 2 * dx1;
			} else {
				if ((dx < 0 && dy<0) || (dx > 0 && dy > 0)) {
				x = x + 1;
				} else {
				x = x - 1;
				}
			py = py + 2 * (dx1 - dy1);
		    }            // Draw pixel from line span at
		    // currently rasterized position
		goto_pos.x = x;
		goto_pos.y = y;
		goto_point(goto_pos);
		}
	}

	digitalWrite(LASER_PIN, LOW);
}

void goto_point(Position pos) {
	move_motor(pos.x,CSX);
	move_motor(pos.y,CSY);
}

void enable_motor(unsigned char pin){
	digitalWrite(pin, LOW);
	SPI.transfer(31);
	SPI.transfer(5);
	digitalWrite(pin, HIGH);
}

void disable_motor(unsigned char pin){
	digitalWrite(pin, LOW);
	SPI.transfer(31);
	SPI.transfer(0);
	digitalWrite(pin, HIGH);
}
