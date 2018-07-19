#include <Pololu3pi.h>
#include <PololuQTRSensors.h>
#include <OrangutanLCD.h>
#include <OrangutanAnalog.h>
#include <OrangutanLEDs.h>

Pololu3pi robot;

const char welcome_lineA[] PROGMEM = " Pololu";
const char welcome_lineB[] PROGMEM = "3\xf7 Robot";

void setup() {
	robot.init(2000);
	OrangutanLCD::printFromProgramSpace(welcome_lineA);
	OrangutanLCD::gotoXY(0, 1);
  	OrangutanLCD::printFromProgramSpace(welcome_lineA);

  	int bat = OrangutanAnalog::readBatteryMillivolts();
  	OrangutanLCD::clear();
    OrangutanLCD::print(bat);
    OrangutanLCD::print("mV");
}

void loop() {
	OrangutanLEDs::left(LOW);
    OrangutanLEDs::right(LOW);
    delay(500);
    OrangutanLEDs::left(LOW);
    OrangutanLEDs::right(HIGH);
    delay(500);
    OrangutanLEDs::left(HIGH);
    OrangutanLEDs::right(LOW);
    delay(500);
    OrangutanLEDs::left(HIGH);
    OrangutanLEDs::right(HIGH);
    delay(500);
}
