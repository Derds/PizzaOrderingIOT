/*
  Reading multiple RFID tags, simultaneously!
  By: Nathan Seidle @ SparkFun Electronics
  Date: October 3rd, 2016
  https://github.com/sparkfun/Simultaneous_RFID_Tag_Reader

  Write a new EPC (Electronic Product Code) to a tag
  Add an easy to read tag ID. There are 12 bytes of available memory for this

  Extra info can be stored in the User Data portion of the tag.

  LED Blink code from http://www.arduino.cc/en/Tutorial/Blink
*/

#include <SoftwareSerial.h> //Used for transmitting to the device

SoftwareSerial softSerial(2, 3); //RX, TX

#include "SparkFun_UHF_RFID_Reader.h" //Library for controlling the M6E Nano module
RFID nano; //Create instance of module

void setup()
{
  Serial.begin(115200); //begin serial at this baud rate

    //LED will light on success
  pinMode(LED_BUILTIN, OUTPUT); //builtin LED pin on arduino uno is pin 13. Neg leg to GND

  //Initial Blink
  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH voltage level)
  delay(1000);                       // wait a second
  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off (voltage LOW)
  
  while (!Serial); //wait for serial to be open then print
  Serial.println();
  Serial.println("Initializing...");

  if (setupNano(38400) == false) //Configure nano to run at 38400bps
  {
    Serial.println("Module failed to respond. Please check wiring."); //if set up nano returns false - usually if power disconnected
    while (1); //Freeze!
  }
  nano.setRegion(REGION_EUROPE); //Set region to Europe
  
  nano.setReadPower(500); //5.00 dBm. Higher values may cause USB port to brown out
  //Max Read TX Power is 27.00 dBm and may cause temperature-limit throttling
  nano.setWritePower(500); //5.00 dBm. Higher values may cause USB port to brown out
  //Max Write TX Power is 27.00 dBm and may cause temperature-limit throttling
}

void loop()
{
  Serial.println("Get all tags out of the area. Press a key to write EPC to first detected tag.");
  if (Serial.available()) Serial.read(); //Clear any chars in the incoming buffer (like a newline char)
  while (!Serial.available()); //Wait for user to send a character
  Serial.read(); //Throw away the user's character

  //"Hello" Does not work. "Hell" will be recorded. You can only write even number of bytes
  char stringEPC[] = "Olives"; //You can only write even number of bytes UP TO 12 BYTES
  byte responseType = nano.writeTagEPC(stringEPC, sizeof(stringEPC) - 1); //The -1 shaves off the \0 found at the end of string
  
  //char hexEPC[] = {0xFF, 0x2D, 0x03, 0x54}; //You can only write even number of bytes
  //byte responseType = nano.writeTagEPC(hexEPC, sizeof(hexEPC));

  if (responseType == RESPONSE_SUCCESS)
  {
    Serial.println("New EPC Written!");
    //Success Blink
    digitalWrite(LED_BUILTIN, HIGH);   
    delay(1000);                       
    digitalWrite(LED_BUILTIN, LOW);
  }
  else
    Serial.println("Failed write");
    Serial.println(responseType);
}

//Gracefully handles a reader that is already configured and already reading continuously
//Because Stream does not have a .begin() we have to do this outside the library
boolean setupNano(long baudRate)
{
  nano.begin(softSerial); //Tell the library to communicate over software serial port

  //Test to see if we are already connected to a module
  //This would be the case if the Arduino has been reprogrammed and the module has stayed powered
  softSerial.begin(baudRate); //For this test, assume module is already at our desired baud rate
  while (!softSerial); //Wait for port to open

  //About 200ms from power on the module will send its firmware version at 115200. We need to ignore this.
  while (softSerial.available()) softSerial.read();
  nano.getVersion();

  if (nano.msg[0] == ERROR_WRONG_OPCODE_RESPONSE)
  {
    //This happens if the baud rate is correct but the module is doing a ccontinuous read
    nano.stopReading();
    Serial.println(F("Module continuously reading. Asking it to stop..."));
    delay(1500);
  }
  else
  {
    //The module did not respond so assume it's just been powered on and communicating at 115200bps
    softSerial.begin(115200); //Start software serial at 115200
    nano.setBaud(baudRate); //Tell the module to go to the chosen baud rate. Ignore the response msg
    softSerial.begin(baudRate); //Start the software serial port, this time at user's chosen baud rate
  }

  //Test the connection
  nano.getVersion();
  if (nano.msg[0] != ALL_GOOD) return (false); //Something is not right

  //The M6E has these settings no matter what
  nano.setTagProtocol(); //Set protocol to GEN2
  nano.setAntennaPort(); //Set TX/RX antenna ports to 1

  return (true); //Setup completed successfully
}
