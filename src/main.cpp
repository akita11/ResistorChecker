#include <Arduino.h>
#include <M5Unified.h>
#include <FastLED.h>

#define NUM_LEDS 1
#define LED_DATA_PIN 35
static CRGB leds[NUM_LEDS];

// G1/G2/V/D
#define PIN_ADC  1
#define PIN_BZ   2

void showLED(int r, int g, int b) {
	leds[0] = CRGB(r, g, b);
	FastLED.show();
}

// read voltage [mV]
float readVoltage(int samples = 10) 
{

	long sum = 0;
	for (int i = 0; i < samples; ++i) {
		sum += analogRead(PIN_ADC);
	}
	return sum / (float)samples * 3300 / 4095.0;
}

float calcResistance(float V, float Rref) {
	if (V == 0) return(1e9); // disconnected, R=inf
	return Rref * (3300.0 - V) / V;
}

uint8_t range = 0;

float Rmin = 900;
float Rmax = 1100;
float Rdisconnect = 1e6;

void setup() {
	auto cfg = M5.config();
	M5.begin(cfg);
	FastLED.addLeds<WS2812B, LED_DATA_PIN, GRB>(leds, NUM_LEDS);
	FastLED.setBrightness(128);
	FastLED.clear();
	showLED(0, 255, 0);  // green
	pinMode(PIN_BZ, OUTPUT);

}

void loop() {
	M5.update();
	if (M5.BtnA.wasClicked()) {
		range = (range + 1) %2;
		if (range == 0) showLED(0, 255, 0);  // green
		else showLED(0, 0, 255); // blue
		delay(500);
	}
	float V = readVoltage();
	float R = calcResistance(V, 1000);
	printf("%f / %f\n", V, R);

	if (R > Rmin && R < Rmax){
		// OK
		showLED(0, 20,  0); // green
		tone(PIN_BZ, 880); // 880Hz
	}
	else if (R < Rmin) {
		// R is too small
		showLED(20, 0, 0); // red
		tone(PIN_BZ, 440); // 440Hz
	}
	else if (R > Rmax && R < Rdisconnect) {
		// R is too large
		showLED(20, 0, 20); // purple
		tone(PIN_BZ, 440); // 440Hz
	}
	else {
		// disconnected
		showLED(0, 0, 0); // black
		noTone(PIN_BZ);
	}
}
