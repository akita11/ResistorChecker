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

float Rstd = 1000; // [ohm]
float Rmin = Rstd * 0.9; // -10%
float Rmax = Rstd * 1.1; // +10%
float Rdisconnect = 1e6; // 1MEG

void setTone(int freq) {
	printf("%d\n", freq);
	if (freq > 0) {
		ledcWriteTone(0, freq);
	} else {
		ledcWrite(0, 0);
	}
}

void setup() {
	auto cfg = M5.config();
	M5.begin(cfg);
	FastLED.addLeds<WS2812B, LED_DATA_PIN, GRB>(leds, NUM_LEDS);
	FastLED.setBrightness(128);
	FastLED.clear();
	showLED(0, 0, 10);  // blue (idle)
  ledcSetup(0, 24000, 11);
	ledcWrite(0, 0);
  ledcAttachPin(PIN_BZ, 0);
}

void loop() {
	M5.update();
	if (M5.BtnA.wasClicked()) {
		range = (range + 1) %2;
		delay(500);
	}
	float V = readVoltage();
	float R = calcResistance(V, Rstd);
//	printf("%f / %f\n", V, R);

	if (R > Rmin && R < Rmax){
		// OK
		showLED(0, 40,  0); // green
		setTone(440);
	}
	else if (R < Rmin) {
		// R is too small
		showLED(40, 0, 0); // red
		setTone(880);
	}
	else if (R > Rmax && R < Rdisconnect) {
		// R is too large
		showLED(20, 0, 20); // purple
		setTone(220);
	}
	else {
		// disconnected / idle
		showLED(0, 0, 10); // blue (idle)
		setTone(0);
	}
}
