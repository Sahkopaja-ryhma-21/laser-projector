/*#include "Constants.h"
#include "Motors.h"
#include "Arduino.h"
#include "Constants.h"
#include "ActionQueue.h"
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
		goto_point(goto_pos, GOTO_WAIT_TIME);

		// Rasterize the line
		for (x; x < pos.x || x > pos.x; x+=i) {
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
			goto_point(goto_pos, GOTO_WAIT_TIME);
		}

	} else { // The line is Y-axis dominant        

		// line is drawn bottom to top
		if(dy<0)
			i = -1;
			
		Position goto_pos;
		goto_pos.x = x;
		goto_pos.y = y;
		goto_point(goto_pos, GOTO_WAIT_TIME);
			
		// Rasterize the line
		for (y; y < pos.y || y > pos.y; y+=i) {
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
	SPI.transfer(30);
	SPI.transfer(0);
	SPI.transfer(5);
	digitalWrite(pin, HIGH);
}

void flip_motor(unsigned char pin){
  digitalWrite(pin,LOW);
  SPI.transfer(11);
  SPI.transfer(1);
  digitalWrite(pin,HIGH);
}

void disable_motor(unsigned char pin){
	digitalWrite(pin, LOW);
	SPI.transfer(31);
	SPI.transfer(0);
	digitalWrite(pin, HIGH);
}
*/