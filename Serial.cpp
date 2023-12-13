#include "Serial.h"
#include "Arduino.h"
#include "Constants.h"
#include "Motors.h"
#include "Command.h"

String input = "";

#define DATA_BEGIN_CHAR 'd'

// This fuction is used to reset the Arduino
void(* resetFunc) (void) = 0;

void useSerial(InstructionList *listptr){
	if (Serial.available() > 0){
		char c = Serial.read();
		if (c == DATA_BEGIN_CHAR){
			read_data(listptr);
			enable_motor(CSX);
			enable_motor(CSY);
			delay(1000);
			return;
		}
		if (c==';') {
			execute(input);
			input = "";
		} else {
			input.concat(c);
		}
	}
}

// Reads a new InstructionList from serial
void read_data(InstructionList *listptr) {
	listptr->clear();
	short i = 0;
	while (true){
		while (Serial.available()<3){
		}

		unsigned char comm = Serial.read();
		unsigned char posx = Serial.read();
		unsigned char posy = Serial.read();

		Instruction instruction;
		instruction.command = from_char(comm);
		instruction.pos = create_position(posx, posy);
		listptr->addInstruction(instruction);
		// full zeroes tell that the input is over and time to get to work.
		if ((comm | posy | posx)  == 0){
			digitalWrite(STATUS_LED, HIGH);
			listptr->finalize();
			return;
		}
		i+=3;
		// This is a guard for overflowing the buffer. In that case the Arduino is reset to hopefully tell the user that the problem is with their instructions
		if (i>=3*LIST_SIZE) resetFunc();
	}
}

// Takes a string of format: "<PARAMETER> <VALUE>"
void execute(String s){
	Serial.print("Executing: ");
	Serial.println(s);
	s.trim();
	// This is the command that will be executed
	String parameter;
	// This is the parameter for the command
	int value;
	for (int i = 0; i < s.length(); i++){
		if(s[i] == ' '){
			String possible_value = s.substring(i);
			possible_value.trim();
			// Check that it is actually convertable
			for (int j = 0; j < possible_value.length();j++){
				if (!isDigit(possible_value[j])){
					Serial.print(possible_value);
					Serial.print(" ");
					Serial.println("is not a possible value for parameter");
					return;
				}
			}
			value = possible_value.toInt();
			break;
		}
		parameter += s[i];
	}
	if (parameter == "gwt"){
		GOTO_WAIT_TIME = value;
		return;
	}
	if (parameter == "mwt"){
		MOVING_WAIT_MULTIPLIER = value;
		return;
	}
	if (parameter == "mxe"){
		if (value){enable_motor(CSX);} else {disable_motor(CSX);}
		return;
	}
	if (parameter == "mye"){
		if (value){enable_motor(CSY);} else {disable_motor(CSY);}
		return;
	}
	Serial.print(parameter);
	Serial.println(" is not a valid command");
	return;
}
