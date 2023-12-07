#include "Constants.h"
#include "Position.h"
#include "Arduino.h"
#include <math.h>

const byte ITERATIONS = 9;

unsigned short Position::distance(Position &another){
	long xdiff = abs(this->x - another.x);
	long ydiff = abs(this->y - another.y);
	return bsqrt(xdiff*xdiff + ydiff*ydiff);
}

unsigned short Position::time_between(Position &another){
	unsigned short maxDist = max(abs(this->x - another.x), abs(this->y - another.y));
	return MOVING_WAIT_MULTIPLIER*GOTO_WAIT_TIME*maxDist;
}

unsigned short Position::bsqrt(unsigned long n){
	if (n <= 1){
		return n;
	}

	int guess = n/2;

	for (int i = 0; i < ITERATIONS; i++){
		guess = (guess + n/guess)/2;
	}
	return guess;
};

Position create_position(unsigned char x,unsigned char y){
	Position pos;
	pos.x = x;
	pos.y = y;
	return pos;
}
