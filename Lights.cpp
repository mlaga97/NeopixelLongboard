#include "Arduino.h"
#include "Lights.h"

void renderFront() {
  for(int i = 3; i < 8; i++) {
    strip1.setPixelColor(i+112, Adafruit_NeoPixel::Color(255, 255, 255));
  }
  for(int i = 23; i < 28; i++) {
    strip1.setPixelColor(i+112, Adafruit_NeoPixel::Color(255, 255, 255));
  }
}

void renderRear() {
  for(int i = 3; i < 8; i++) {
    strip2.setPixelColor(i, Adafruit_NeoPixel::Color(128, 0, 0));
  }
  for(int i = 23; i < 28; i++) {
    strip2.setPixelColor(i, Adafruit_NeoPixel::Color(128, 0, 0));
  }
}

void renderSides(int pattern) {
	if(pattern == 0) {
		for(int i = 0; i < 112; i++) {
			strip1.setPixelColor(i, Adafruit_NeoPixel::Color(0,0,0));
			strip2.setPixelColor(i+32, Adafruit_NeoPixel::Color(0,0,0));
		}
	}
	
	if(pattern == 1) {
		for(int i = 0; i < 112; i++) {
			if(i % spacing == 0) {
				strip1.setPixelColor(i, Adafruit_NeoPixel::Color(rgb_rVal,rgb_gVal,rgb_bVal));
				strip2.setPixelColor(i+32, Adafruit_NeoPixel::Color(rgb_rVal,rgb_gVal,rgb_bVal));
			} else {
				strip1.setPixelColor(i, Adafruit_NeoPixel::Color(0,0,0));
				strip2.setPixelColor(i+32, Adafruit_NeoPixel::Color(0,0,0));
			}
		}
	}
	
	if(pattern == 2) {
		long pixelPosition = ticks * 15.54; // 4.25"/tick * 144 pixels/meter = 15.44 pixels/tick

		for(int i = 0; i < 112; i++) {
			if(direction) {
				strip1.setPixelColor(i, pattern_RainbowRails(i + pixelPosition, true));
				strip2.setPixelColor(i+32, pattern_RainbowRails(i + pixelPosition, false));
			} else {
				strip1.setPixelColor(111-i, pattern_RainbowRails(i + pixelPosition, true));
				strip2.setPixelColor((111-i)+32, pattern_RainbowRails(i + pixelPosition, false));
			}
		}
	}
}
