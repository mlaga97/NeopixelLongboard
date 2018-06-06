#include "Arduino.h"
#include "RainbowRails.h"
#include <Adafruit_NeoPixel.h>

uint32_t pattern_RainbowRails(long int pos, bool side) {
  int rel = pos % RAINBOW_RAILS_PATTERN_LENGTH;

  if(rel > (0*RAINBOW_RAILS_SUBPATTERN_OFFSET) && rel < (0*RAINBOW_RAILS_SUBPATTERN_OFFSET)+RAINBOW_RAILS_SUBPATTERN_LENGTH)
    return Adafruit_NeoPixel::Color(RAINBOW_RAILS_BRIGHTNESS, 0, 0);
  else if(rel > (1*RAINBOW_RAILS_SUBPATTERN_OFFSET) && rel < (1*RAINBOW_RAILS_SUBPATTERN_OFFSET)+RAINBOW_RAILS_SUBPATTERN_LENGTH)
    return Adafruit_NeoPixel::Color(0, RAINBOW_RAILS_BRIGHTNESS, 0);
  else if(rel > (2*RAINBOW_RAILS_SUBPATTERN_OFFSET) && rel < (2*RAINBOW_RAILS_SUBPATTERN_OFFSET)+RAINBOW_RAILS_SUBPATTERN_LENGTH)
    return Adafruit_NeoPixel::Color(0, 0, RAINBOW_RAILS_BRIGHTNESS);
  else
    return Adafruit_NeoPixel::Color(0, 0, 0);
};
