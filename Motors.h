#include "Position.h"

#define CSX 10
#define CSY 9
#define t 10
#define WAIT_TIME 10
#define LASER_PIN 4

void move_motor(unsigned char, unsigned char);

void draw_line(Position, Position);

void goto_point(Position);
