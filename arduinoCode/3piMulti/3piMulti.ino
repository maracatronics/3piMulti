

/*----- Battery.ino -----*/

#include <Pololu3pi.h>
#include <PololuQTRSensors.h>
#include <OrangutanLCD.h>
#include <OrangutanAnalog.h>
#include <OrangutanLEDs.h>

Pololu3pi robot0;

const char welcome_lineA0[] PROGMEM = " Pololu";
const char welcome_lineB0[] PROGMEM = "3\xf7 Robot";

void setup0() {
	robot0.init(2000);
	OrangutanLCD::printFromProgramSpace(welcome_lineA0);
	OrangutanLCD::gotoXY(0, 1);
  	OrangutanLCD::printFromProgramSpace(welcome_lineA0);

  	int bat0 = OrangutanAnalog::readBatteryMillivolts();
  	OrangutanLCD::clear();
    OrangutanLCD::print(bat0);
    OrangutanLCD::print("mV");
}

void loop0() {
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



/*----- Simple1.ino -----*/

#include <Pololu3pi.h>
#include <PololuQTRSensors.h>
#include <OrangutanLCD.h>
#include <OrangutanMotors.h>

Pololu3pi robot1;

const char welcome_lineA1[] PROGMEM = " Pololu";
const char welcome_lineB1[] PROGMEM = "3\xf7 Robot";

void setup1() {
	robot1.init(2000);
	OrangutanLCD::printFromProgramSpace(welcome_lineA1);
	OrangutanLCD::gotoXY(0, 1);
  	OrangutanLCD::printFromProgramSpace(welcome_lineB1);
}

void loop1() {
	OrangutanMotors::setSpeeds(40, -40);
	delay(2000);
	OrangutanMotors::setSpeeds(-40, 40);
	delay(2000);
}



/*----- SimpleLineFollower.ino -----*/

/*
 * Simple3piLineFollower - demo code for the Pololu 3pi Robot
 * 
 * This code will follow a black line on a white background, using a
 * very simple algorithm.  It demonstrates auto-calibration and use of
 * the 3pi IR sensors, motor control, bar graphs using custom
 * characters, and music playback, making it a good starting point for
 * developing your own more competitive line follower.
 *
 * http://www.pololu.com/docs/0J21
 * http://www.pololu.com
 * http://forum.pololu.com
 *
 */

// The 3pi include file must be at the beginning of any program that
// uses the Pololu AVR library and 3pi.  Pololu3pi.h includes all of the
// other Orangutan Arduino libraries that can be used to control the
// on-board hardware such as LCD, buzzer, and motor drivers.
#include <Pololu3pi.h>
#include <PololuQTRSensors.h>
#include <OrangutanMotors.h>
#include <OrangutanAnalog.h>
#include <OrangutanLEDs.h>
#include <OrangutanLCD.h>
#include <OrangutanPushbuttons.h>
#include <OrangutanBuzzer.h>

Pololu3pi robot2;
unsigned int sensors2[5]; // an array to hold sensor values

// This include file allows data to be stored in program space.  The
// ATmega168 has 16k of program space compared to 1k of RAM, so large
// pieces of static data should be stored in program space.
#include <avr/pgmspace.h>

// Introductory messages.  The "PROGMEM" identifier causes the data to
// go into program space.
const char welcome_line12[] PROGMEM = " Pololu";
const char welcome_line22[] PROGMEM = "3\xf7 Robot";
const char demo_name_line12[] PROGMEM = "Line";
const char demo_name_line22[] PROGMEM = "follower";

// A couple of simple tunes, stored in program space.
const char welcome2[] PROGMEM = ">g32>>c32";
const char go2[] PROGMEM = "L16 cdegreg4";

// Data for generating the characters used in load_custom_characters
// and display_readings.  By reading levels[] starting at various
// offsets, we can generate all of the 7 extra characters needed for a
// bargraph.  This is also stored in program space.
const char levels2[] PROGMEM = {
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111
};

// This function loads custom characters into the LCD.  Up to 8
// characters can be loaded; we use them for 7 levels of a bar graph.
void load_custom_characters2()
{
  OrangutanLCD::loadCustomCharacter(levels2 + 0, 0); // no offset, e.g. one bar
  OrangutanLCD::loadCustomCharacter(levels2 + 1, 1); // two bars
  OrangutanLCD::loadCustomCharacter(levels2 + 2, 2); // etc...
  OrangutanLCD::loadCustomCharacter(levels2 + 3, 3);
  OrangutanLCD::loadCustomCharacter(levels2 + 4, 4);
  OrangutanLCD::loadCustomCharacter(levels2 + 5, 5);
  OrangutanLCD::loadCustomCharacter(levels2 + 6, 6);
  OrangutanLCD::clear(); // the LCD must be cleared for the characters to take effect
}

// This function displays the sensor readings using a bar graph.
void display_readings2(const unsigned int *calibrated_values2)
{
  unsigned char i2;

  for (i2=0;i2<5;i2++) {
    // Initialize the array of characters that we will use for the
    // graph.  Using the space, an extra copy of the one-bar
    // character, and character 255 (a full black box), we get 10
    // characters in the array.
    const char display_characters2[10] = { ' ', 0, 0, 1, 2, 3, 4, 5, 6, 255 };

    // The variable c will have values from 0 to 9, since
    // calibrated values are in the range of 0 to 1000, and
    // 1000/101 is 9 with integer math.
    char c2 = display_characters2[calibrated_values2[i2] / 101];

    // Display the bar graph character.
    OrangutanLCD::print(c2);
  }
}

// Initializes the 3pi, displays a welcome message, calibrates, and
// plays the initial music.  This function is automatically called
// by the Arduino framework at the start of program execution.
void setup2()
{
  unsigned int counter2; // used as a simple timer

  // This must be called at the beginning of 3pi code, to set up the
  // sensors.  We use a value of 2000 for the timeout, which
  // corresponds to 2000*0.4 us = 0.8 ms on our 20 MHz processor.
  robot2.init(2000);

  load_custom_characters2(); // load the custom characters

  // Play welcome music and display a message
  OrangutanLCD::printFromProgramSpace(welcome_line12);
  OrangutanLCD::gotoXY(0, 1);
  OrangutanLCD::printFromProgramSpace(welcome_line22);
  OrangutanBuzzer::playFromProgramSpace(welcome2);
  delay(1000);

  OrangutanLCD::clear();
  OrangutanLCD::printFromProgramSpace(demo_name_line12);
  OrangutanLCD::gotoXY(0, 1);
  OrangutanLCD::printFromProgramSpace(demo_name_line22);
  delay(1000);

  // Display battery voltage and wait for button press
  while (!OrangutanPushbuttons::isPressed(BUTTON_B))
  {
    int bat2 = OrangutanAnalog::readBatteryMillivolts();

    OrangutanLCD::clear();
    OrangutanLCD::print(bat2);
    OrangutanLCD::print("mV");
    OrangutanLCD::gotoXY(0, 1);
    OrangutanLCD::print("Press B");

    delay(100);
  }

  // Always wait for the button to be released so that 3pi doesn't
  // start moving until your hand is away from it.
  OrangutanPushbuttons::waitForRelease(BUTTON_B);
  delay(1000);

  // Auto-calibration: turn right and left while calibrating the
  // sensors.
  for (counter2=0; counter2<80; counter2++)
  {
    if (counter2 < 20 || counter2 >= 60)
      OrangutanMotors::setSpeeds(40, -40);
    else
      OrangutanMotors::setSpeeds(-40, 40);

    // This function records a set of sensor readings and keeps
    // track of the minimum and maximum values encountered.  The
    // IR_EMITTERS_ON argument means that the IR LEDs will be
    // turned on during the reading, which is usually what you
    // want.
    robot2.calibrateLineSensors(IR_EMITTERS_ON);

    // Since our counter runs to 80, the total delay will be
    // 80*20 = 1600 ms.
    delay(20);
  }
  OrangutanMotors::setSpeeds(0, 0);

  // Display calibrated values as a bar graph.
  while (!OrangutanPushbuttons::isPressed(BUTTON_B))
  {
    // Read the sensor values and get the position measurement.
    unsigned int position2 = robot2.readLine(sensors2, IR_EMITTERS_ON);

    // Display the position measurement, which will go from 0
    // (when the leftmost sensor is over the line) to 4000 (when
    // the rightmost sensor is over the line) on the 3pi, along
    // with a bar graph of the sensor readings.  This allows you
    // to make sure the robot is ready to go.
    OrangutanLCD::clear();
    OrangutanLCD::print(position2);
    OrangutanLCD::gotoXY(0, 1);
    display_readings2(sensors2);

    delay(100);
  }
  OrangutanPushbuttons::waitForRelease(BUTTON_B);

  OrangutanLCD::clear();

  OrangutanLCD::print("Go!");       

  // Play music and wait for it to finish before we start driving.
  OrangutanBuzzer::playFromProgramSpace(go2);
  while(OrangutanBuzzer::isPlaying());
}

// The main function.  This function is repeatedly called by
// the Arduino framework.
void loop2()
{
  // Get the position of the line.  Note that we *must* provide
  // the "sensors" argument to read_line() here, even though we
  // are not interested in the individual sensor readings.
  unsigned int position2 = robot2.readLine(sensors2, IR_EMITTERS_ON);
  if (position2 < 1000)
  {
    // We are far to the right of the line: turn left.

    // Set the right motor to 100 and the left motor to zero,
    // to do a sharp turn to the left.  Note that the maximum
    // value of either motor speed is 255, so we are driving
    // it at just about 40% of the max.
    OrangutanMotors::setSpeeds(0, 100);

    // Just for fun, indicate the direction we are turning on
    // the LEDs.
    OrangutanLEDs::left(HIGH);
    OrangutanLEDs::right(LOW);
  }
  else if (position2 < 3000)
  {
    // We are somewhat close to being centered on the line:
    // drive straight.
    OrangutanMotors::setSpeeds(100, 100);
    OrangutanLEDs::left(HIGH);
    OrangutanLEDs::right(HIGH);
  }
  else
  {
    // We are far to the left of the line: turn right.
    OrangutanMotors::setSpeeds(100, 0);
    OrangutanLEDs::left(LOW);
    OrangutanLEDs::right(HIGH);
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
			case 0: OrangutanLCD::print("Battery.ino"); break;
			case 1: OrangutanLCD::print("Simple1.ino"); break;
			case 2: OrangutanLCD::print("SimpleLineFollower.ino"); break;
			default: break;
		}
		if(OrangutanPushbuttons::isPressed(BUTTON_A)) {
			OrangutanPushbuttons::waitForRelease(BUTTON_A);
			option = (option - 1) % 3;
		}
		else if (OrangutanPushbuttons::isPressed(BUTTON_B)) {
			OrangutanPushbuttons::waitForRelease(BUTTON_B);
			option = (option + 1) % 3;
		}
		else if (OrangutanPushbuttons::isPressed(BUTTON_C)) {
			OrangutanPushbuttons::waitForRelease(BUTTON_C);
			switch (option) {
				case 0: mainsetup = &setup0; mainloop = &loop0; break;
				case 1: mainsetup = &setup1; mainloop = &loop1; break;
				case 2: mainsetup = &setup2; mainloop = &loop2; break;
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
