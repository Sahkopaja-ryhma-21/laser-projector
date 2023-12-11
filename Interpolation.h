#include "Position.h"
#include "ActionQueue.h"

#define CSX 10
#define CSY 9
#define TIME_CONSTANT 10
#define WAIT_TIME 10
#define LASER_PIN 4
#define STATUS_LED 7

struct ActionQueue;

void draw_line(ActionQueue &actions, Position, Position);
