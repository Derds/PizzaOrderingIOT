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

int numNotes; // Number of notes in the melody
void setup() {
  // put your setup code here, to run once:
  strip.begin();
  CircuitPlayground.begin();  // initialize the CP library
  playTones(helloRise[], noteDurations1[]);
  for (int i = 3; i >= 0; i--) {
    //callRainbowChase();
    playTones(success[], noteDurations2[]);
  }

}

void loop() {
  if(CircuitPlayground.leftButton()) {
  playTones(C_Scale[], noteDurations2[]);
  }
  if(CircuitPlayground.rightButton()) {
    playTones(ohNo[], noteDurations2[]);
    fadeRainbow(strip.Color(102, 0, 102)); //begin purple
  }
}

static void playTones(int[] melody, int[] durations){
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


static void fadeRainbow(uint32_t c){
    strip.show(); //  all pixels start off
    colourString(c);
    for(int i = 0; i<=255; i+=20){
      colourString(strip.Color(i, 0, i));
        for(int j = 255; j>=0; j-=15){
         colourString(strip.Color(i, j, i));
               for(int k = 0; k<=255; k+=20){
                   colourString(strip.Color(i,j,k));
                   colourString(strip.Color(j,0,k));
                   colourString(strip.Color(k,j,0));
                   colourString(strip.Color(i,j,k));
                   colourString(strip.Color(0,i,k));
                   colourString(strip.Color(j,i,k));
                   colourString(strip.Color(0,j,k));
                   colourString(strip.Color(0,0,k));
                   colourString(strip.Color(j,i,0));
                }
               colourString(strip.Color(0, 0, 0));
      }
      colourString(strip.Color(0, 0, 0));
    }
}

static void colourString(uint32_t c){
  for(int i = 0; i<=N_LEDS; i++){
       strip.setPixelColor(i, c);
       strip.show();
       delay(15);
    }
}

static void callRainbowChase(){
  chase(strip.Color(255, 0, 0)); // Red
  chase(strip.Color(255,69,0)); // Orange Red
  chase(strip.Color(255,165,0)); // Orange
  chase(strip.Color(255,215,0)); // Yellow //255 255 0
  chase(strip.Color(173,255,47)); // YellowGreen
  chase(strip.Color(0, 255, 0)); // Green
  chase(strip.Color(0, 128, 0)); // Dark Green
  chase(strip.Color(32,178,170)); // BlueGreen
  chase(strip.Color(135,206,250)); // Light Blue
  chase(strip.Color(0, 0, 255)); // Blue
  chase(strip.Color(25,25,112)); // Midnight Blue
  chase(strip.Color(75,0,130)); // Indigo
  chase(strip.Color(128,0,128)); // Purple
  chase(strip.Color(186,85,211)); // Light Purple
  chase(strip.Color(255,0,255)); // Magenta
  chase(strip.Color(250,128,114)); // Pink
  chase(strip.Color(139,0,0)); // Dark Red  
}


static void chase(uint32_t c) {
  for(uint16_t i=0; i<strip.numPixels()+5; i++) {
      strip.setPixelColor(i  , c); // Draw new pixel
      strip.setPixelColor(i-5, 0); // Erase pixel a few steps back
      strip.show();
      delay(50);
  }
}