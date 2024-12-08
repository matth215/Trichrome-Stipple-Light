/*
Version 3.0
Developed for Lab64 Makerspace @ Stanford
Initial release 2024
Authors: Matthew Hamilton, Caleb Matthews, Ege Turan

Includes new animations and modes!
Mode 0 - Off
Mode 1 - RGB Display 
Mode 2 - Color Shift
Mode 3 - Noise flicker
*/

//NOTE: Check that pins A1 and A0 are the ones you selected for your project, if not edit lines 20 and 21 as necessary

#include <FastLED.h> // we’ll use the FastLED library

#define PPS 6 // Pixels Per Strip
#define NUM_STRIPS 3 // Establish number of strips (always 3 for the TSL project)
#define INTENSITY_PIN A1 //Establish Analog In A1 as volume
#define MODE_PIN A0   //Establish Analog In A0 as mode selector 

// Define the pins connected to the data lines of each LED strip
#define DATA_PIN_RED 1  // Red channel
#define DATA_PIN_GRN 2  // Green channel
#define DATA_PIN_BLU 3  // Blue channel

// Create CRGB array for all LED pixels
CRGB leds[PPS * NUM_STRIPS];

// Initialize integers for color shift (mode2)
int strip = 0;
int frame = 0;
int sign = 1;
int bound = 50;
int dividend = 0;

// Create the animation functions here with 3 cases
// Lights off Mode
void case0() {
  fill_solid(leds, PPS * NUM_STRIPS, CRGB::Black);
}

// RGB Mode - just display the R, G, B channels as normal. 
void case1(int intensity) {
  fill_solid(leds, 6, CHSV(0, 255, intensity));
  fill_solid(leds + 6, 6, CHSV(95, 255, intensity));
  fill_solid(leds + 12, 6, CHSV(160, 255, intensity));
}

// Color Shift Mode
void case2(int intensity) {
  frame = frame + sign;
  strip = dividend % 3;

  if (strip == 0) {
    fill_solid(leds, 6, CHSV(0 + frame, 255, intensity));
    fill_solid(leds + 6, 6, CHSV(95, 255, intensity));
    fill_solid(leds + 12, 6, CHSV(160, 255, intensity));
  }
  else if (strip == 1) {
    fill_solid(leds, 6, CHSV(0, 255, intensity));
    fill_solid(leds + 6, 6, CHSV(95 + frame, 255, intensity));
    fill_solid(leds + 12, 6, CHSV(160, 255, intensity));
  }
  else if (strip == 2) {
    fill_solid(leds, 6, CHSV(0, 255, intensity));
    fill_solid(leds + 6, 6, CHSV(95, 255, intensity));
    fill_solid(leds + 12, 6, CHSV(160 + frame, 255, intensity));
  }
  if (frame > 49) { //if the animation frame is at the top of its cycle i.e. frame = 50, switch the direction
    sign = sign * (-1);
  }
  else if (frame < 1) { //if the animation frame is at the end of its cycle i.e. frame = 0 and color is normal..
    sign = sign * (-1);
    dividend++; //increment the dividend 
    delay(1000);
  } 
  if (dividend > 32000) {
    dividend = 0;
  }
}

// Noise Mode
void case3(int intensity) {
  // Define base hues for each strip (Red, Green, Blue)
  uint8_t baseHue[3] = {0, 95, 160}; 

  for (int strip = 0; strip < NUM_STRIPS; strip++) {
    for (int pixel = 0; pixel < PPS; pixel++) {
      // Calculate the index of the current pixel
      int index = strip * PPS + pixel;

      // Randomly decide if this pixel should flicker
      bool flicker = random8() < 25;  // Adjust the probability here

      if (flicker) {
        // Dim the LED to 50% of the brightness
        leds[index] = CHSV(baseHue[strip], 255, intensity / 2);
      } else {
        // Set the pixel color with full intensity
        leds[index] = CHSV(baseHue[strip], 255, intensity);
      }
    }
  }
}



// Set up the LED array. This also plays an animation when connected to power.
void setup() {
  //Serial.begin(9600);
  pinMode(INTENSITY_PIN, INPUT);
  pinMode(MODE_PIN, INPUT);

  // Add LED strips with their respective data pins
  FastLED.addLeds<WS2812, DATA_PIN_RED>(leds, 0, 6);
  FastLED.addLeds<WS2812, DATA_PIN_GRN>(leds, 6, 12);
  FastLED.addLeds<WS2812, DATA_PIN_BLU>(leds, 12, 18);

  //Start-up Animation here
  //From black, fill all leds with white, then transition to RGB color
  fill_solid(leds, PPS * NUM_STRIPS, CRGB::Black);
  FastLED.show();
  delay(10);

  for (int hue = 0; hue < 255; hue++) {
    fill_solid(leds, PPS * NUM_STRIPS, CHSV(0, 0, hue));
    FastLED.show();
    delay(10);
  }
  delay(1000);
  fill_solid(leds, PPS * NUM_STRIPS, CRGB(175, 175, 175));
  FastLED.show();

  //Fade to the RGB scheme 
  for (int shift = 0; shift < 255; shift++) {
    fill_solid(leds, 6, CRGB(255, 255 - shift, 255 - shift));
    fill_solid(leds + 6, 6, CRGB(255 - shift, 255, 255 - shift));
    fill_solid(leds + 12, 6, CRGB(255 - shift, 255 - shift, 255));
    FastLED.show();
    delay(5);
  }
}

// Check the mode knob and the volume knob, then run through the corresponding display modes. 
void loop() {
  int volume = map(analogRead(INTENSITY_PIN), 0, 1024, 25, 255); //read the volume knob
  int mode = map(analogRead(MODE_PIN), 0, 1024, 0, 4);  //read the mode knob

  if (mode == 0) {
    case0();
    FastLED.show();
    delay(10);
  }
  
  else if (mode == 1) {
    case1(volume);
    FastLED.show();
    delay(10);
  }

  else if (mode == 2) {
    case2(volume);
    FastLED.show();
    delay(3);
  }

  else if (mode == 3) {
    case3(volume);
    FastLED.show();
    delay(25);
  }

  delay(25);
}

 
