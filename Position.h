#pragma once
#include <Arduino.h>

class Position{
	private:
		unsigned short bsqrt(unsigned long n);
	public:
		Position(byte x1, byte y1);
		byte x;
		byte y;
		// Returns the approximate distance between two positions.
		unsigned short distance(Position &another);
};
