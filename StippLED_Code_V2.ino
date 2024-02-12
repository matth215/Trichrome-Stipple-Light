/*
Version 2.1
Developed for Lab64 Makerspace @ Stanford
Initial release 2024
Author: Matthew Hamilton, Caleb Matthews, Ege Turan
*/
#include <FastLED.h> // we’ll use the FastLED library
#define NUM_PIXELS_PER_STRIP 6 // Establish num of pixels in each strip
#define NUM_STRIPS 3 // Establish num of strips
#define INTENSITY_PIN A0
#define MODE_PIN A1
// Define the pins connected to the data lines of each LED strip
#define DATA_PIN_RED 1  // Red channel
#define DATA_PIN_GRN 2  // Green channel
#define DATA_PIN_BLU 3  // Blue channel
// Create CRGB array for all LED pixels
CRGB leds[NUM_PIXELS_PER_STRIP * NUM_STRIPS];
void setup() {
  //Serial.begin(9600);
  pinMode(INTENSITY_PIN, INPUT);
  pinMode(MODE_PIN, INPUT);
  // Add LED strips with their respective data pins
  FastLED.addLeds<NEOPIXEL, DATA_PIN_RED>(leds, 0, 6);
  FastLED.addLeds<NEOPIXEL, DATA_PIN_GRN>(leds, 6, 12);
  FastLED.addLeds<NEOPIXEL, DATA_PIN_BLU>(leds, 12, 18);
}
void loop() {
  int strength = map(analogRead(A0), 0, 1024, 0, 255);
  int mode = map(analogRead(A1), 0, 1024, 0, 4);
  for (int strip = 0; strip < NUM_STRIPS; strip++) { // index to each LED
    for (int pixel = 0; pixel < NUM_PIXELS_PER_STRIP; pixel++) {
      if (strip == 0) {
        leds[pixel + (strip * NUM_PIXELS_PER_STRIP)] = CRGB(strength*(mode == 0 || mode == 1),0,0); // set each LED to color
      } else if (strip == 1) {
        leds[pixel + (strip * NUM_PIXELS_PER_STRIP)] = CRGB(0,strength*(mode == 0 || mode == 2),0); // set each LED to color
      } else if (strip == 2){
        leds[pixel + (strip * NUM_PIXELS_PER_STRIP)] = CRGB(0,0,strength*(mode == 0 || mode == 3)); // set each LED to color
      }
    }
  }
  FastLED.show(); // update LEDs
}
