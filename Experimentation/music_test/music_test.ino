// Adafruit Circuit Playground - Melody    Support Open Source, buy at Adafruit
//   2016-08-06 Version 1  by Mike Barela for Adafruit Industries
//   Adapted from melody by Tom Igoe on arduino.cc
// Uses the CircuitPlayground library to easily use the full functionality of the board

#include <Adafruit_CircuitPlayground.h>
#include "pitches.h"

int melody[] = {                            // specific notes in the melody
NOTE_E7, NOTE_E7, 1, NOTE_E7,
  1, NOTE_C7, NOTE_E7, 1,
  NOTE_G7, 1, 1, 1,
  NOTE_G6, 1, 1, 1,
 
  NOTE_C7, 1, 1, NOTE_G6,
  1, 1, NOTE_E6, 1,
  1, NOTE_A6, 1, NOTE_B6,
  1, NOTE_AS6, NOTE_A6, 1,
 
  NOTE_G6, NOTE_E7, NOTE_G7,
  NOTE_A7, 1, NOTE_F7, NOTE_G7,
  1, NOTE_E7, 1, NOTE_C7,
  NOTE_D7, NOTE_B6, 1, 1,
 
  NOTE_C7, 1, 1, NOTE_G6,
  1, 1, NOTE_E6, 1,
  1, NOTE_A6, 1, NOTE_B6,
  1, NOTE_AS6, NOTE_A6, 1,
 
  NOTE_G6, NOTE_E7, NOTE_G7,
  NOTE_A7, 1, NOTE_F7, NOTE_G7,
  1, NOTE_E7, 1, NOTE_C7,
  NOTE_D7, NOTE_B6, 1, 1  
 };
int numNotes; // Number of notes in the melody
 
int noteDurations[] = {     // note durations
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,
 
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,
 
  9, 9, 9,
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,
 
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,
 
  9, 9, 9,
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,
};
 

void setup() {
  CircuitPlayground.begin();  // initialize the CP library
    numNotes = sizeof(melody)/sizeof(int);  // number of notes we are playing
}

void loop() {
  if(CircuitPlayground.rightButton()) {   // play when we press the right button
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
}
