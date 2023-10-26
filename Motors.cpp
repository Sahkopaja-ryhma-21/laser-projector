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
	goto_point(pos);
	// FIX: Master must do interpolation.
	digitalWrite(LASER_PIN, LOW);
}

void goto_point(Position pos) {
	move_motor(pos.x,CSX);
	move_motor(pos.y,CSY);
}
