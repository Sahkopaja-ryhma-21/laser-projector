#pragma once

#include "Arduino.h"
#include "Command.h"

void read_data(InstructionList* listptr);
void useSerial(InstructionList* listptr);
void execute(String s);
