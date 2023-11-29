#pragma once

struct Position{
	private:
		unsigned short bsqrt(unsigned long n);
	public:
		unsigned char x;
		unsigned char y;
		// Returns the approximate distance between two positions.
		unsigned short distance(Position &another);
};

Position create_position(char ,char);
