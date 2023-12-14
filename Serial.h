#pragma once

#include "Command.h"
#include "Arduino.h"

class ActionQueue; //forward declaration to avoid having to #include in header

void useSerial(InstructionList *listprt, ActionQueue &actions);
void execute(ActionQueue &actions, String s);
