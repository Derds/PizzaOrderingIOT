//Circuit Playground express code
/*
*On power -> Celebratory Noise
*->Cycle through rainbow colours on neopixels
*
*Chase Neopixel Code Reference: https://learn.adafruit.com/neopixel-painter/test-neopixel-strip
*Extra colours from https://www.rapidtables.com/web/color/index.html
*
*Reference for music code
*Adafruit Circuit Playground - Melody    Support Open Source, buy at Adafruit
//   2016-08-06 Version 1  by Mike Barela for Adafruit Industries
//   Adapted from melody by Tom Igoe on arduino.cc
*/

#include <Adafruit_NeoPixel.h> //include neopixel library     
#define NEOPIXEL_PIN A1 //Which pin are neopixels attached to (likely A1 or A0)
#define N_LEDS 30 //length of neopixels
//setup strip
Adafruit_NeoPixel strip = Adafruit_NeoPixel(N_LEDS, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);

#include <Adafruit_CircuitPlayground.h> //include plaground library
#include "pitches.h" //include pitches definition for music

//define the melodies
int helloRise[] = {NOTE_C3, NOTE_E3, NOTE_G3, NOTE_C4};
int noteDurations1[] = {9, 9, 9, 12};
int ohNo[] = {NOTE_FS4, NOTE_C4};
int noteDurations2[] = {10, 12};
int success[] = {NOTE_C4,NOTE_C4,NOTE_C4,NOTE_G4};
int noteDurations3[] = {9, 9, 9, 12};
int C_Scale[] = {NOTE_C4,NOTE_D4,NOTE_E4,NOTE_F4,NOTE_G4,NOTE_A4,NOTE_B4,NOTE_C5};
int noteDurations4[] = {12, 12,12, 12,12, 12,12, 12};

int numNotes;
#define playTones(melody, noteDurations)
//int melody[8];
//int noteDurations[8];

void setup() {
  // put your setup code here, to run once:
  strip.begin();
  CircuitPlayground.begin();  // initialize the CP library

  playTones(helloRise, noteDurations1);
  delay(1000);
  playTones(C_Scale, noteDurations4);
}

void loop() {
  // testing success and failure feedback
  if(CircuitPlayground.leftButton()) {
    successFeedback();
  }
  if(CircuitPlayground.rightButton()) {
    failureFeedback();
  }
}

static void successFeedback(){
  playTones(success, noteDurations3);
  chase(strip.Color(0, 255, 0)); // Green
}
static void failureFeedback(){
  playTones(ohNo, noteDurations2);
  chase(strip.Color(255, 0, 0)); // Red
}

static void chase(uint32_t c) {
  for(uint16_t i=0; i<strip.numPixels()+5; i++) {
      strip.setPixelColor(i  , c); // Draw new pixel
      strip.setPixelColor(i-5, 0); // Erase pixel a few steps back
      strip.show();
      delay(50);
  }
  
static void playTones(int melody[], int noteDurations[]){
  numNotes = sizeof(melody)/sizeof(int);  // number of notes we are playing
   for (int thisNote = 0; thisNote < numNotes; thisNote++) { // play notes of the melody
      // to calculate the note duration, take one second divided by the note type.
      //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
      int noteDuration = 1000 / noteDurations[thisNote];
      CircuitPlayground.playTone(melody[thisNote], noteDuration);

      // to distinguish the notes, set a minimum time between them.
      //   the note's duration + 30% seems to work well:
      int pauseBetweenNotes = noteDuration * 1.30;
      delay(pauseBetweenNotes);
    }
}
