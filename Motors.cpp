#include "Constants.h"
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

	int dx1 = dx < 0 ? -dx : dx;
	int dy1 = dy < 0 ? -dy : dy;

	int px = (dy1*2)-dx1; 
	int py = (dx1*2)-dy1;

	int x;
	int y;
	int xe;
	int ye;

	// x-dominant
	if(dy1 <= dx1){

		// left to right
		if(dx>=0){
			x = last_pos.x;
			y = last_pos.y;
			xe = pos.x;
		} else { // right to left
			x = pos.x;
			y = pos.y;
			xe = last_pos.x;
		}
		Position goto_pos;
		goto_pos.x = x;
		goto_pos.y = y;
		goto_point(goto_pos, GOTO_WAIT_TIME);

		// Rasterize the line
		for (int i = 0; x < xe; i++) {
			x++;            // Deal with octants...
			if (px < 0) {
				px += 2 * dy1;
			} else {
				if ((dx < 0 && dy < 0) || (dx > 0 && dy > 0)) {
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
			goto_point(goto_pos, GOTO_WAIT_TIME);
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
		goto_point(goto_pos, GOTO_WAIT_TIME);
			
		// Rasterize the line
		for (int i = 0; y < ye; i++) {
			y = y + 1;            // Deal with octants...
			if (py <= 0) {
				py += 2 * dx1;
			} else {
				if ((dx < 0 && dy<0) || (dx > 0 && dy > 0)) {
				x++;
				} else {
				x--;
				}
			py += 2 * (dx1 - dy1);
		    }            // Draw pixel from line span at
		    // currently rasterized position
		goto_pos.x = x;
		goto_pos.y = y;
		goto_point(goto_pos, GOTO_WAIT_TIME);
		}
	}

	digitalWrite(LASER_PIN, LOW);
}

void goto_point(Position pos, unsigned short time) {
	move_motor(pos.x,CSX);
	move_motor(pos.y,CSY);
	delayMicroseconds(2*time);
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
