#include "Position.h"
#include "Arduino.h"

const byte ITERATIONS = 9;

unsigned short Position::distance(Position &another){
	long xdiff = abs(this->x - another.x);
	long ydiff = abs(this->y - another.y);
	return bsqrt(xdiff*xdiff + ydiff*ydiff);
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

Position create_position(char x,char y){
	Position pos;
	pos.x = x;
	pos.y = y;
	return pos;
}
