#include <OrangutanPushbuttons.h>


/*----- medidorDeLoop.ino -----*/


#include <Pololu3pi.h>
#include <PololuQTRSensors.h>
#include <OrangutanLCD.h>

Pololu3pi robot0;

long int lastTimer0;
int counter0 = 1000;

const char welcome_lineA0[] PROGMEM = " Pololu";
const char welcome_lineB0[] PROGMEM = "3\xf7 Robot";

void setup0() {
	robot0.init(2000);
	OrangutanLCD::printFromProgramSpace(welcome_lineA0);
	OrangutanLCD::gotoXY(0, 1);
  OrangutanLCD::printFromProgramSpace(welcome_lineB0);
  lastTimer0=micros();
}

void loop0() {
    
    if(!(counter0--)){
        long int delta0 = micros()-lastTimer0;
        OrangutanLCD::gotoXY(0, 1);
      	OrangutanLCD::print(delta0);
      	OrangutanLCD::print("       ");
        lastTimer0 = micros();
        counter0 = 1000;
    }
}



/*----- MENU -----*/

void(*mainsetup)();
void(*mainloop)();

void setup() {
	int option = 0;
	bool selected = false;
	while (!selected) {
		OrangutanLCD::clear();
		OrangutanLCD::gotoXY(0, 0);
		OrangutanLCD::print("Prog. ");
		OrangutanLCD::print(option + 1);
		OrangutanLCD::gotoXY(0, 1);
		switch (option) {
			case 0: OrangutanLCD::print("medidorDeLoop.ino"); break;
			default: break;
		}
		if(OrangutanPushbuttons::isPressed(BUTTON_A)) {
			OrangutanPushbuttons::waitForRelease(BUTTON_A);
			option = (option - 1) % 1;
		}
		else if (OrangutanPushbuttons::isPressed(BUTTON_B)) {
			OrangutanPushbuttons::waitForRelease(BUTTON_B);
			option = (option + 1) % 1;
		}
		else if (OrangutanPushbuttons::isPressed(BUTTON_C)) {
			OrangutanPushbuttons::waitForRelease(BUTTON_C);
			switch (option) {
				case 0: mainsetup = &setup0; mainloop = &loop0; break;
				default: break;
			}
			selected = true;
		}
		delay(100);
	}
	mainsetup();
}

void loop() {
	mainloop();
}
