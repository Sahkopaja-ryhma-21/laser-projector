#include "Serial.h"
#include "Arduino.h"
#include "Constants.h"
#include "Motors.h"

String input = "";

void useSerial(){
	if (Serial.available() > 0){
		char c = Serial.read();
		switch (c){
			case '\n': 
				execute(input);
				input = "";
				break;
			default:
				input += c;
				break;
		}
	}
}

// Takes a string of format: "<PARAMETER> <VALUE>"
void execute(String s){
	s.trim();
	// This is the command that will be executed
	String parameter;
	// This is the parameter for the command
	int value;
	for (int i = 0; i < s.length(); i++){
		if(s[i] == ' '){
			String possible_value = s.substring(i);
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
