#include "Serial.h"
#include "Arduino.h"
#include "Constants.h"
#include "Command.h"
#include "ActionQueue.h"

String input = "";

#define DATA_BEGIN_CHAR 'd'

// This fuction is used to reset the Arduino
void(* resetFunc) (void) = 0;

void useSerial(InstructionList *listptr, ActionQueue &actions){
	if (Serial.available() > 0){
		char c = Serial.read();
		if (c == DATA_BEGIN_CHAR){
			listptr->read_data();
			actions.pushMotorEnable(Recipient::X, ConfigSlot::CONFIG_SLOT_1);
			actions.pushMotorEnable(Recipient::Y, ConfigSlot::CONFIG_SLOT_1);
			delay(1000);
			return;
		}
		if (c==';') {
			execute(actions,input);
			input = "";
		} else {
			input.concat(c);
		}
	}
}


// Takes a string of format: "<PARAMETER> <VALUE>"
void execute(ActionQueue &actions, String s){
	Serial.print("Executing: ");
	Serial.println(s);
	s.trim();
	// This is the command that will be executed
	String parameter;
	// This is the parameter for the command
	int value = 0;
	for (unsigned i = 0; i < s.length(); i++){
		if(s[i] == ' '){
			String possible_value = s.substring(i);
			possible_value.trim();
			// Check that it is actually convertable
			for (unsigned j = 0; j < possible_value.length();j++){
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

		if (value)
        	actions.pushMotorEnable(Recipient::X, ConfigSlot::CONFIG_SLOT_1);
        	else
        	actions.pushMotorDisable(Recipient::X);
		return;
	}
	if (parameter == "mye"){
		if (value)
        	actions.pushMotorEnable(Recipient::Y, ConfigSlot::CONFIG_SLOT_1);
        	else
        	actions.pushMotorDisable(Recipient::Y);
		return;
	}
	Serial.print(parameter);
	Serial.println(" is not a valid command");
	return;
}
