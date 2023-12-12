#include "Position.h"

#define CSX 10
#define CSY 9
#define TIME_CONSTANT 10
#define WAIT_TIME 10
#define LASER_PIN 4
#define STATUS_LED 7


void draw_line(Position, Position);

void goto_point(Position, unsigned short);

void enable_motor(unsigned char);
<<<<<<< HEAD

=======
void flip_motor(unsigned char);
>>>>>>> parent of b031c96 (implement action queue for more consistent output timing)
void disable_motor(unsigned char);
