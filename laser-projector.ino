#include "Position.h"
#include "Command.h"
#include "Interpolation.h"
#include "ActionQueue.h"
#include <SPI.h>
#include <stdint.h>

int8_t amogus[] = {
    30,  -50, 1,
    30, -100, 1,
   100, -100, 1,
   100,   30, 1,
    75,   80, 1,
    30,  100, 1,
   -30,  100, 1,
   -75,   80, 1,
  -100,   30, 1,
  -100, -100, 1,
   -30, -100, 1,
   -30,  -50, 1,
     0,  -40, 1, //amoguksen ääriviivan viimeinen piste jalkojen välissä
    30,  -50, 0, //väliasema 1 siirtymässä
    50,   30, 0, //väliasema 2 siirtymässä
     0,   30, 0, //siirtymä naama-aukon alkupisteeseen
   -50,   30, 1, //naama-aukko
   -60,   55, 1,
   -50,   80, 1,
     0,   80, 1,
    50,   80, 1,
    60,   55, 1,
    50,   30, 1,
     0,   30, 1,
   -50,   30, 0,
   -30,  -50, 0,
     0,  -40, 0
};

ActionQueue actions;
const InstructionList commands;
void(* resetFunc) (void) = 0;

ISR(TIMER1_OVF_vect)
{
    actions.executeCommands();
}

void setup() {
    sei(); //enable interrupts
    pinMode(STATUS_LED, OUTPUT);
    actions.begin();
    uint8_t resetSequence[] = {30, 1, 5};
	actions.pushSpiPacket(Recipient::X, resetSequence, sizeof(resetSequence));
    actions.pushSpiPacket(Recipient::Y, resetSequence, sizeof(resetSequence));
    actions.pushLaserState(true);
    while(1)
    {
        for(int n = 0; n < 50; ++n)
        {
            while(actions.getLength() > actions.getCapacity() / 2);
            actions.pushSpiPacket(Recipient::X, 0);
            actions.pushSpiPacket(Recipient::Y, 0);
            actions.pushWait();
        }
        for(int n = 0; n < 50; ++n)
        {
            while(actions.getLength() > actions.getCapacity() / 2);
            actions.pushSpiPacket(Recipient::X, 255);
            actions.pushSpiPacket(Recipient::Y, 0);
            actions.pushWait();
        }
        for(int n = 0; n < 50; ++n)
        {
            while(actions.getLength() > actions.getCapacity() / 2);
            actions.pushSpiPacket(Recipient::X, 255);
            actions.pushSpiPacket(Recipient::Y, 255);
            actions.pushWait();
        }
        for(int n = 0; n < 50; ++n)
        {
            while(actions.getLength() > actions.getCapacity() / 2);
            actions.pushSpiPacket(Recipient::X, 0);
            actions.pushSpiPacket(Recipient::Y, 255);
            actions.pushWait();
        }
    }
	//Serial.begin(9600);
	//commands = read_data();
    commands = read_amogus();

	delay(2000);
}

void loop() {
    //avoid overflowing the action buffer
	commands.executeNext(actions);
}

InstructionList read_data() {
	InstructionList list;
	short i = 0;
	while (true){
        while (Serial.available()<3){}

        unsigned char comm = Serial.read();
        unsigned char posx = Serial.read();
        unsigned char posy = Serial.read();

        // full zeroes tell that the input is over and time to get to work.

        Instruction instruction;
        instruction.command = from_char(comm);
        instruction.pos = create_position(posx, posy);
        list.addInstruction(instruction);
        if ((comm | posy | posx)  == 0){
            digitalWrite(STATUS_LED, HIGH);
            list.finalize();
            return list;
        }
        i+=3;
        if (i>=1026) resetFunc();
	}
}

InstructionList read_amogus() {
	InstructionList list;
	for(uint8_t i = 0; i < sizeof(amogus) / 3; ++i)
    {
        unsigned char comm = 1;//amogus[i * 3 + 2];
        unsigned char posx = (int16_t)amogus[i * 3 + 0] + 128;
        unsigned char posy = (int16_t)amogus[i * 3 + 1] + 128;

        if(comm == 0)
            comm = 1;
        else
            comm = 2;
        // full zeroes tell that the input is over and time to get to work.

        Instruction instruction;
        instruction.command = from_char(comm);
        instruction.pos = create_position(posx, posy);
        list.addInstruction(instruction);
	}
    Instruction instruction;
    instruction.command = from_char(0);
    instruction.pos = create_position(128, 128);
    list.addInstruction(instruction);
    digitalWrite(STATUS_LED, HIGH);
    list.finalize();
    return list;
}
