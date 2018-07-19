#include <Pololu3pi.h>
#include <PololuQTRSensors.h>
#include <OrangutanLCD.h>
#include <OrangutanMotors.h>

Pololu3pi robot;

const char welcome_lineA[] PROGMEM = " Pololu";
const char welcome_lineB[] PROGMEM = "3\xf7 Robot";

void setup() {
	robot.init(2000);
	OrangutanLCD::printFromProgramSpace(welcome_lineA);
	OrangutanLCD::gotoXY(0, 1);
  	OrangutanLCD::printFromProgramSpace(welcome_lineB);
}

void loop() {
	OrangutanMotors::setSpeeds(40, -40);
	delay(2000);
	OrangutanMotors::setSpeeds(-40, 40);
	delay(2000);
}
