
#include <Adafruit_CircuitPlayground.h>

void setup() {
  // put your setup code here, to run once:
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
    delay(200);
  Serial.print("Capsense #2: "); Serial.println(CircuitPlayground.readCap(2));
  Serial.print("Capsense #3: "); Serial.println(CircuitPlayground.readCap(3));
    delay(200);
  Serial.print("Capsense #6: "); Serial.println(CircuitPlayground.readCap(6));
  Serial.print("Capsense #9: "); Serial.println(CircuitPlayground.readCap(9));
    delay(200);
  Serial.print("Capsense #10: "); Serial.println(CircuitPlayground.readCap(10));
  Serial.print("Capsense #12: "); Serial.println(CircuitPlayground.readCap(0));
  delay(2000);
}
