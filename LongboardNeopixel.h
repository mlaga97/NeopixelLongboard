#pragma once

#include "Arduino.h"

#include <Adafruit_NeoPixel.h>
#include <Wire.h>
#include <ESP8266WiFi.h>

#include "Lights.h"

extern Adafruit_NeoPixel strip1;
extern Adafruit_NeoPixel strip2;

extern volatile uint32_t ticks;
extern volatile uint32_t lastElapsed;
extern volatile uint32_t lastMillis;

extern int16_t AcX, AcY, AcZ, Tmp, GyX, GyY, GyZ;

extern int spacing;
extern uint8_t rgb_rVal;
extern uint8_t rgb_gVal;
extern uint8_t rgb_bVal;

extern bool direction;
