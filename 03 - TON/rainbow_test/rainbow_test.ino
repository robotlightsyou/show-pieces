#include <FastLED.h>

#define NUM_LEDS 16

#define DATA_PIN D7

CRGB leds[NUM_LEDS];

void setup(){
  
FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);
  };  
  uint32_t Wheel(uint16_t WheelPos){
  byte r, g, b;
  switch(WheelPos / 128)
  {
    case 0:
      r = 127 - WheelPos % 128; // red down
      g = WheelPos % 128;       // green up
      b = 0;                    // blue off
      break;
    case 1:
      g = 127 - WheelPos % 128; // green down
      b = WheelPos % 128;       // blue up
      r = 0;                    // red off
      break;
    case 2:
      b = 127 - WheelPos % 128; // blue down
      r = WheelPos % 128;       // red up
      g = 0;                    // green off
      break;
  }
}


