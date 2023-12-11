#include "Serial.h"
#include "Arduino.h"

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

void execute(String s){
	// TODO: Implement
}
