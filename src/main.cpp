#include <Arduino.h>
#include <M5Unified.h>
#include <FastLED.h>

#define NUM_LEDS 1
static CRGB leds[NUM_LEDS];

/*
// for ATOMS3 Lite
// G1/G2/V/D
#define PIN_ADC  1
#define PIN_BZ   2
#define PIN_LED  35
*/
// for ATOM Lite
// G1/G2/V/D
#define PIN_ADC  32
#define PIN_BZ   26
#define PIN_LED  27

void showLED(int r, int g, int b) {
	leds[0] = CRGB(r, g, b);
	FastLED.show();
}

// read voltage [mV]
float readVoltage(int samples = 10) 
{
	long sum = 0;
	for (int i = 0; i < samples; ++i) {
//		sum += analogRead(PIN_ADC);
		sum += analogReadMilliVolts(PIN_ADC);
}
//	return sum / (float)samples * 3300 / 4095.0;
	return sum / (float)samples; // [mV]
}

float calcResistance(float V, float Rref) {
	if (V == 0) return(1e9); // disconnected, R=inf
	return Rref * (3300.0 - V) / V;
}

uint8_t range = 0;

float Rstd = 1000; // [ohm]
float Rmin = Rstd * 0.9; // -10%
float Rmax = Rstd * 1.1; // +10%
float Rdisconnect = Rstd * 20;

void setTone(int freq) {
	if (freq > 0) {
		ledcWriteTone(0, freq);
	} else {
		ledcWrite(0, 0);
	}
}

void setup() {
	auto cfg = M5.config();
	M5.begin(cfg);
	FastLED.addLeds<WS2812B, PIN_LED, GRB>(leds, NUM_LEDS);
	FastLED.setBrightness(128);
	FastLED.clear();
	showLED(0, 0, 10);  // blue (idle)
  ledcSetup(0, 24000, 11);
	ledcWrite(0, 0);
  ledcAttachPin(PIN_BZ, 0);
}

uint8_t st = 0, st0 = 0;
uint8_t cnt = 0;

void loop() {
/*
	M5.update();
	if (M5.BtnA.wasClicked()) {
		range = (range + 1) % 4;
		switch(range) {
			case 0 : setTone(220); printf("220Hz\n"); break;
			case 1 : setTone(440); printf("440Hz\n"); break;
			case 2 : setTone(880); printf("880Hz\n"); break;
			case 3 : setTone(0); printf("stop\n"); break;
		}
		delay(500);
	}
*/
	float V = readVoltage();
	float R = calcResistance(V, Rstd);
	printf("%f / %f\n", V, R);

	if (R > Rmin && R < Rmax){
		// OK
		st = 1;
	}
	else if (R < Rmin) {
		// R is too small
		st = 2;
	}
	else if (R > Rmax && R < Rdisconnect) {
		// R is too large
		st = 3;
	}
	else {
		// disconnected / idle
		st = 0;
	}

	if (st != st0){
		switch (st){
			case 0: setTone(0);   showLED( 0,  0, 10); break; // blue
			case 1: setTone(440); showLED( 0, 40,  0); break; // green
			case 2: setTone(880); showLED(40,  0,  0); break; // red
			case 3: setTone(220); showLED(20,  0, 20); break; // purple
		}
		st0 = st;
	}
}

