/* ======================================================================================
          Project: Arduino LED Light Box Paparazzi in the Rain sketch
           Author: Scott C
          Created: 18th August 2015
      Arduino IDE: 1.6.4
LED pixel chipset: WS8201
          Website: http://arduinobasics.blogspot.com/p/arduino-basics-projects-page.html
      Description: This sketch will create a randomised raindrop effect with intermittent blue flashes of light on a strand of 12mm Diffused 
              Flat Digital RGB LED Pixels (25 LEDs).  This project makes use of the 
             FastLED library v3.0.3 (http://fastled.io/). 
            
            
            LEDs from OpenLab Australia: 
            http://openlab.com.au/shop/12mm-diffused-flat-digital-rgb-led-pixels-strand-of-25/
            
            LED Pixel Strip        Arduino UNO
            ==================================
            Red Wire  = VCC   ==>                  (5V from external power supply)
            Blue Wire = GND   ==> GND
          Yellow Wire = Data  ==> Digital Pin 2
           Green Wire = Clock ==> Digital Pin 3
           
           
========================================================================================= */

#include "FastLED.h"            // This sketch uses the FastLED library                                    
 
#define DATA 0              // Data connected to Digital Pin 2 on the Arduino                                     
#define CLOCK 3               // Clock connected to Digital Pin 3 on the Arduino                             
#define LED_TYPE WS2812B         // This LED strip uses the WS2801 chipset.                                   
#define NUM_LEDS 24             // There are 25 LEDs on this strand

//Global Variables
byte brightness[NUM_LEDS];
byte hue[NUM_LEDS];
struct CRGB leds[NUM_LEDS];     // the array of LEDs    
byte startVal[NUM_LEDS];
byte minHue =0;
byte maxHue =40;
byte maxBrightness = 100;
    

void setup() {
  //If you have a different LED strip, you may need to modify the next line
  //For this LED strand, you need to define both the data and the clock connections
  LEDS.addLeds<LED_TYPE, DATA, RGB>(leds, NUM_LEDS); 
  FastLED.clear();
  random16_add_entropy( random());


  for(int i = 0; i<NUM_LEDS; i++){
    startVal[i] = random8();
    hue[i] = random(minHue, maxHue);
  }
}


void loop() {
  delay(10);
  for(int i = 0; i<NUM_LEDS; i++){
    startVal[i]++;
    brightness[i] = sin8(startVal[i]);
    leds[i] = CHSV(hue[i], 255, map(brightness[i],0,255,0,maxBrightness));
    if(random(1000)<100){
      startVal[i] = startVal[i]+2;
    }
    if(brightness[i] <3){
      hue[i] = random(minHue, maxHue);
      minHue++;
      maxHue++;
      if(random8(100)<20){
        leds[i]=250;
        delay(50);
      }
    }
  }
  
  FastLED.show();
   
} 
