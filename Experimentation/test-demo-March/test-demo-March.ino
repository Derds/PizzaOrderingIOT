// Demo program for testing library and board - flip the switch to turn on/off buzzer

#include <Adafruit_CircuitPlayground.h>

// we light one pixel at a time, this is our counter
uint8_t pixeln = 0;

void setup() {
  //while (!Serial);
  Serial.begin(9600);
  Serial.println("Circuit Playground test!");

  CircuitPlayground.begin();
}


void loop() {
  // test Red #13 LED
  CircuitPlayground.redLED(HIGH);
  delay(200);
  CircuitPlayground.redLED(LOW);
  delay(200);


  /************* TEST CAPTOUCH */
  if (! CircuitPlayground.isExpress()) {  // CPX does not have this captouch pin
    Serial.print("Capsense #0: "); Serial.println(CircuitPlayground.readCap(0));
  }
  Serial.print("Capsense #1: "); Serial.println(CircuitPlayground.readCap(1));
  Serial.print("Capsense #2: "); Serial.println(CircuitPlayground.readCap(2));
  Serial.print("Capsense #3: "); Serial.println(CircuitPlayground.readCap(3));
  Serial.print("Capsense #6: "); Serial.println(CircuitPlayground.readCap(6));
  Serial.print("Capsense #9: "); Serial.println(CircuitPlayground.readCap(9));
  Serial.print("Capsense #10: "); Serial.println(CircuitPlayground.readCap(10));
  Serial.print("Capsense #12: "); Serial.println(CircuitPlayground.readCap(12));
  
  /************* TEST SLIDE SWITCH */
  if (CircuitPlayground.slideSwitch()) {
    Serial.println("Slide to the left");
  } else {
    Serial.println("Slide to the right");
    CircuitPlayground.playTone(500 + pixeln * 500, 100);
  }
  delay(1000);


  /************* TEST BOTH BUTTONS */
  if (CircuitPlayground.leftButton()) {
      Serial.println("Left button pressed!");
      /************* TEST 10 NEOPIXELS */
      CircuitPlayground.setPixelColor(pixeln++, CircuitPlayground.colorWheel(25 * pixeln));
      if (pixeln == 11) {
        pixeln = 0;
        CircuitPlayground.clearPixels();
      }
  
   CircuitPlayground.playTone(260, 100); //Play C
   CircuitPlayground.playTone(290, 100); //Play D
    CircuitPlayground.playTone(330, 100); //Play E
     CircuitPlayground.playTone(350, 100); //Play F
      CircuitPlayground.playTone(390, 100); //Play G
       CircuitPlayground.playTone(440, 100); //Play A
        CircuitPlayground.playTone(490, 100); //Play B
         CircuitPlayground.playTone(520, 100); //Play C
  }
  if (CircuitPlayground.rightButton()) {
    Serial.println("Right button pressed!");
        CircuitPlayground.playTone(490, 100); //Play B
        CircuitPlayground.playTone(440, 100); //Play A
        CircuitPlayground.playTone(390, 100); //Play G
        CircuitPlayground.playTone(440, 100); //Play A
        CircuitPlayground.playTone(490, 100); //Play B
        CircuitPlayground.playTone(490, 100); //Play B
        CircuitPlayground.playTone(490, 130); //Play B
        delay(100);
        CircuitPlayground.playTone(440, 100); //Play A
        CircuitPlayground.playTone(440, 100); //Play A
        CircuitPlayground.playTone(440, 130); //Play A
        delay(100);
        CircuitPlayground.playTone(490, 100); //Play B
        CircuitPlayground.playTone(490, 100); //Play B
        CircuitPlayground.playTone(490, 130); //Play B
        delay(100);
        CircuitPlayground.playTone(490, 100); //Play B
        CircuitPlayground.playTone(440, 100); //Play A
        CircuitPlayground.playTone(390, 100); //Play G
        CircuitPlayground.playTone(440, 100); //Play A
        CircuitPlayground.playTone(490, 100); //Play B
        CircuitPlayground.playTone(490, 100); //Play B
        CircuitPlayground.playTone(490, 130); //Play B
        delay(100);
        CircuitPlayground.playTone(490, 80); //Play B
        CircuitPlayground.playTone(440, 100); //Play A
        CircuitPlayground.playTone(440, 100); //Play A
        CircuitPlayground.playTone(490, 100); //Play B
        CircuitPlayground.playTone(440, 100); //Play A
        CircuitPlayground.playTone(390, 130); //Play G
  }

  /************* TEST LIGHT SENSOR */
  Serial.print("Light sensor: ");
  Serial.println(CircuitPlayground.lightSensor());

  /************* TEST SOUND SENSOR */
  Serial.print("Sound sensor: ");
  Serial.println(CircuitPlayground.mic.soundPressureLevel(10));

  /************* TEST ACCEL */
  // Display the results (acceleration is measured in m/s*s)
  Serial.print("X: "); Serial.print(CircuitPlayground.motionX());
  Serial.print(" \tY: "); Serial.print(CircuitPlayground.motionY());
  Serial.print(" \tZ: "); Serial.print(CircuitPlayground.motionZ());
  Serial.println(" m/s^2");

  /************* TEST THERMISTOR */
  Serial.print("Temperature ");
  Serial.print(CircuitPlayground.temperature());
  Serial.println(" *C");
}
