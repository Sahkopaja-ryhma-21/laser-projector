#include "Motors.h"
#include "Arduino.h"
#include <SPI.h>

void move_motor(unsigned char pos,unsigned char pin){
	digitalWrite(pin, LOW);
	SPI.transfer(pos);
	digitalWrite(pin, HIGH);
}

void draw_line(Position p1, Position p2){
	digitalWrite(LASER_PIN, LOW);
	goto_point(p1);
	digitalWrite(LASER_PIN, HIGH);
	goto_point(p2);
	// FIX: Master must do interpolation.
	delay(p1.distance(p2));
	digitalWrite(LASER_PIN, LOW);
}

void goto_point(Position pos) {
	move_motor(pos.x,CSX);
	move_motor(pos.y,CSY);
}
