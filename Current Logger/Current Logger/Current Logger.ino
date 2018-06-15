/*
 Name:		Current_Logger.ino
 Created:	14-Jun-18 11:35:53
 Author:	tilfr
*/

#define pin 23

uint32_t logRate = 1000;
uint32_t x = 0;
float y = 0, z = 0, slope = 1, interjection = 0;

// the setup function runs once when you press reset or power the board
void setup() {
	Serial.begin(9600);
	while (!Serial);
	pinMode(pin, INPUT);
	Serial.printf("Ready to measure with slope: %f and interjection: %f\n", slope, interjection);
	Serial.println("Time [ms], Ampere [mA]");
}

// the loop function runs over and over again until power down or reset
void loop() {
	y += analogRead(pin) * slope + interjection;
	z++;

	if (millis() - x >= logRate) {
		Serial.printf("%d, %f\n", millis(), y/z);
		x = millis();
		z = 0;
		y = 0;
	}
}
