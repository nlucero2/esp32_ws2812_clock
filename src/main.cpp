#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include "Seven_Segment.h"
#include <time.h>


#define SEVEN_SEG_PIN 48
#define AMPM true
#define SERIAL_BAUD 115200



// create the Seven_Segment object
Seven_Segment display(SEVEN_SEG_PIN, AMPM);

// create the time object
time_t now;


void setup() {

	// initialize serial communication at 115200 bits per second
	Serial.begin(SERIAL_BAUD);

	// set the brightness very low to prevent blinding
	display.setBrightness(5);

	// get the current time
	now = time(NULL);

	// set the time on the seven segment display
	display.setTime(localtime(&now)->tm_hour, localtime(&now)->tm_min, 0x00FFFF);

}// end setup


void loop() {

	// get the current time
	now = time(NULL);

	// get the hours and minutes
	uint8_t hour = localtime(&now)->tm_hour;
	uint8_t minute = localtime(&now)->tm_min;

	// update the time on the seven segment display
	display.updateTime(hour, minute, 0x00FFFF);

	// print the time to verify correctness
	Serial.printf("hour = %d\nminute = %d\n\n", hour, minute);

	// delay
	delay(20 * 1000);


}// end loop
