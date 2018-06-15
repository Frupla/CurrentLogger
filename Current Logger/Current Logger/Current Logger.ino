/*
 Name:		Current_Logger.ino
 Created:	14-Jun-18 11:35:53
 Author:	tilfr
*/

#include "dataLogger.h"

#define pin 23

uint32_t logRate = 10; //[ms]
uint32_t dropRate = 5000; //[ms]
uint32_t x = 0, w = 0;
float v = 0, y = 0, z = 0, slope = 0.023585, interjection = -18.827;
char * message;
int i = 0;
bool measureFlag = false, printFlag = false;

// the setup function runs once when you press reset or power the board
void setup() {
	Serial.begin(9600);
	while (!Serial);
	pinMode(pin, INPUT);
	Serial.printf("Ready to measure with slope: %f and interjection: %f\n", slope, interjection);
	Serial.println("Time [ms], Ampere [mA]");
	initializeLogger();
}

// the loop function runs over and over again until power down or reset
void loop() {
	y += analogRead(pin) * slope + interjection;
	v += analogRead(pin);

	z++;
	if (measureFlag) {
		if (millis() - x >= logRate) {
		if (printFlag) {
			Serial.printf("%d, %f, %f\n", millis(), y / z, v / z);
		}
		dataLog();
		x = millis();
		z = 0;
		y = 0;
		v = 0;
		}

		if (millis() - w >= dropRate) {
		emptyBufferIntoFile();
		}
	}

	if (Serial.available()) {
		switch (Serial.read()){
			case 'r':
				Serial.println("Starting to read");
				readFromFile();
				break;
			case 'c':
				Serial.println("Starting to clear");
				clearSdDrive();
				break;
			case 'm':
				Serial.print("Measure is ");
				if (measureFlag) {
					Serial.println("now off ");
				}else{
					Serial.println("now on");
				}
				measureFlag = !measureFlag;
				break;
			case 'p':
				Serial.print("The board will ");
				if (measureFlag) {
					Serial.print("not ");
				}
				Serial.println("print while measuring");
				printFlag = !printFlag;
				break;
			case 'h':
				Serial.println("You have 5 options:");
				Serial.println("'r' prints all of the measurements, first colum is time [ms], second is ampere [mA]");
				Serial.println("'c' deletes the log on the SD drive");
				Serial.println("'m' starts the measurements");
				Serial.println("'p' toggles if you want to print while measuring or not, by default it is off");
				Serial.println("'h' gives you this list");
				break;
			default : 
				Serial.println("Not understood, we only take 'r', 'c', 'm', 'p' and 'h'");
		}

	}
}

void dataLog() {
	writeToBuffer(millis(), 0);
	writeToBuffer(y / z, 1);
}