#pragma once

#include "Arduino.h"

class ActionQueue; //forward declaration to avoid having to #include in header

void useSerial(ActionQueue &actions);
void execute(ActionQueue &actions, String s);
