#include "LongboardNeopixel.h"

uint32_t pattern_RedShift(long int pos, bool side) {
  if(pos%10 != 0)
    return Adafruit_NeoPixel::Color(0, 0, 0);

  int mod = 0;

  if(AcY > 10000) {
    mod = (AcY-10000)/100;
  } else if (AcY < -10000) {
    mod = (AcY+10000)/100;
  }

  if(side) {
    return Adafruit_NeoPixel::Color(127+mod, 0, 127-mod);
  } else {
    return Adafruit_NeoPixel::Color(127-mod, 0, 127+mod);
  }

  Serial.println(AcY);
}
