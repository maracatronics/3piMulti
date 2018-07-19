
#include <Pololu3pi.h>
#include <PololuQTRSensors.h>
#include <OrangutanLCD.h>

Pololu3pi robot;

long int lastTimer;
int counter = 1000;

const char welcome_lineA[] PROGMEM = " Pololu";
const char welcome_lineB[] PROGMEM = "3\xf7 Robot";

void setup() {
	robot.init(2000);
	OrangutanLCD::printFromProgramSpace(welcome_lineA);
	OrangutanLCD::gotoXY(0, 1);
  OrangutanLCD::printFromProgramSpace(welcome_lineB);
  lastTimer=micros();
}

void loop() {
    
    if(!(counter--)){
        long int delta = micros()-lastTimer;
        OrangutanLCD::gotoXY(0, 1);
      	OrangutanLCD::print(delta);
      	OrangutanLCD::print("       ");
        lastTimer = micros();
        counter = 1000;
    }
}
