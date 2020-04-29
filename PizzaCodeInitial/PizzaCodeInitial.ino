/*Code References: 
  Reading multiple RFID tags, simultaneously!
  By: Nathan Seidle @ SparkFun Electronics
  Date: October 3rd, 2016
  https://github.com/sparkfun/Simultaneous_RFID_Tag_Reader

  Luke Jones - Gitlab

  Serial Read https://www.arduino.cc/reference/en/language/functions/communication/serial/read
  Hex to ASCII - https://forum.arduino.cc/index.php?topic=535140.0
*/

/*TASKS
 * Reading EPC from RFID
 * Then reading user data
*/


/*BUGS
*Not reading EPC tag cleanly - think its to do with serial  https://www.arduino.cc/en/Tutorial/SerialEvent
*             also something about async processes :( https://www.arduino.cc/reference/en/language/functions/communication/serial
*/

//import libraries
#include "SparkFun_UHF_RFID_Reader.h" //Simultaneous RFID reader library
#include <SoftwareSerial.h> //For transmitting to digital pins. https://www.arduino.cc/en/Reference/softwareSerial

/*define variables and create instances*/

SoftwareSerial softSerial(2, 3); //RX, TX - Define pins used by arduino

//NB: ensure serial monitor set to same output as this number
#define baudRate 115200 // Defines the baud rate for communication over the serial port - (bits per second communicated)

//Valid regions: REGION_INDIA, REGION_JAPAN, REGION_CHINA, REGION_EUROPE, REGION_KOREA,REGION_AUSTRALIA, REGION_NEWZEALAND, REGION_NORTHAMERICA
#define region REGION_EUROPE //define local region - to comply to local frequency standards

//Define the readpower of the RFID. (0 - 2700) Recommended: 5.00 dBm. Higher values may cause USB port to brown out
#define RFIDpower 500  //Max Read TX Power is 27.00 dBm and may cause temperature-limit throttling

RFID nano; //Create RFID module instance
#define loopLimit 20 //how many times the RFID tries to read a tag 

void setup() {
  Serial.begin(baudRate); //Begin Serial
  Serial.println("Initialising...");
  Init_RFID();        // initialize RFID shield
}

void loop() {
  // gizmo start
  
  //call RFID
  char mode = ChooseMode(); //will be lowercase
  if(mode == 'a')
  { 
    Serial.println("Please type the EPC that will be written to the tag followed by '.' \n Remember! You can only write even number of bytes");
    //String EPC = Serial.readString();
    //int len = EPC.length();
    //char charBuf[len];
    //EPC.toCharArray(charBuf, len);
    char charBuf[12] = {};
    int len = 12;
    Serial.readBytesUntil('.', charBuf, len);
    WriteEPC(charBuf);
  }
  else if(mode == 'b') ReadEPC();
  else if(mode == 'c') WriteUserData();
  else if(mode == 'd') ReadUserData();
  else if(mode == 'e') ReadBoth();
  //else if(mode == 'f') end program
  //else continue
  //set default mode

}

//Choose Mode for RFID tags
char ChooseMode()
{
  Serial.println("Choose a mode: ");
  Serial.println("A- to Write EPC tag, B - to read EPC tag, C - to write user data, D - to read user data ");
  Serial.println("E- to readEPC and UserData, F- to end program");

  while (!Serial.available()); //Wait for user to send a character
  char incomingChar = 0; // for incoming serial data
  
  // send data only when you receive data:
  if (Serial.available() > 0) {
    // read the incoming byte:
    incomingChar = Serial.read();

    // say what you got:
    Serial.print("I received: ");
    Serial.println(incomingChar);
    Serial.read();
  }

  //return incomingChar.toLowerCase(); //return the string as lowercase
  return incomingChar;
}

//RFID Function
void Init_RFID()
{
    if(!setupNano(baudRate)) //Configure nano to run at 38400bps
  {
    Serial.println("Module failed to respond. Please check wiring.");
    while (1); //Freeze!
  }

  nano.setRegion(region); //Set local region
  nano.setReadPower(RFIDpower); //5.00 dBm. Higher values may cause USB port to brown out
  //Max Read TX Power is 27.00 dBm and may cause temperature-limit throttling
  nano.setWritePower(RFIDpower); //write power same restrictions as read power
  Serial.println("RFID module init successful.");
}

boolean setupNano(long bRate){
  nano.begin(softSerial); //Tell the library to communicate over software serial port

  //Test to see if we are already connected to a module
  softSerial.begin(baudRate);
  while(!softSerial); //Wait for port to open
  
  while(softSerial.available()) softSerial.read();
  
  nano.getVersion();
  if (nano.msg[0] == ERROR_WRONG_OPCODE_RESPONSE )
  {
    nano.stopReading();
    Serial.println(F("Module continuously reading. Asking it to stop..."));
    delay(1000);
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
  
  nano.setTagProtocol(); //Set protocol to GEN2
  nano.setAntennaPort(); //Set TX/RX antenna ports to 1
  return (true); //initialised!
}

void WriteEPC(char myEPC[]){
  Serial.println("Get all tags out of the area. Press a key to write EPC to first detected tag.");
  if (Serial.available()) Serial.read(); //Clear any chars in the incoming buffer (like a newline char)
  while (!Serial.available()); //Wait for user to send a character
  Serial.read(); //Throw away the user's character

  //char stringEPC[] = "Red Pepper"; //You can only write even number of bytes
  byte responseType = nano.writeTagEPC(myEPC, sizeof(myEPC) - 1); //The -1 shaves off the \0 found at the end of string

  //char hexEPC[] = {0xFF, 0x2D, 0x03, 0x54}; //You can only write even number of bytes
  //byte responseType = nano.writeTagEPC(hexEPC, sizeof(hexEPC));

  if (responseType == RESPONSE_SUCCESS)
    Serial.println("New EPC Written!");
  else
    Serial.println("Failed write");
}

void ReadEPC(){
  Serial.println("Press a key to scan for a tag");
  if (Serial.available()) Serial.read(); //Clear any chars in the incoming buffer (like a newline char)
  while (!Serial.available()); //Wait for user to send a character
  Serial.read(); //Throw away the user's character

  byte myEPC[12]; //Most EPCs are 12 bytes - check this for this tag
  byte myEPClength;
  byte responseType = 0;
  char EPCchars[12];

  //int loop = loopLimit; && loop!=0
  while (responseType != RESPONSE_SUCCESS)//RESPONSE_IS_TAGFOUND)
  { 
    myEPClength = sizeof(myEPC); //Length of EPC is modified each time .readTagEPC is called
    responseType = nano.readTagEPC(myEPC, myEPClength, 500); //Scan for a new tag up to 500ms
    Serial.println("Searching for tag");
    //delay(500);                // waits for half a second
    //loop--;
  }

  //Print EPC
  Serial.print(F(" epc["));
  for (byte x = 0 ; x < myEPClength ; x++)
  {
    if (myEPC[x] < 0x10) Serial.print(F("0"));
    Serial.print(myEPC[x], HEX);
    Serial.print(F(" "));
    EPCchars[x] = myEPC[x];
  }
  Serial.println(F("]"));
  //Print as ASCII Text
  for (int i=0; i<myEPClength; i++)
  {
    Serial.print(EPCchars[i] + " "); //prints each character in epc
  }
  Serial.println();
  delay(500);
}

void WriteUserData(){
}

void ReadUserData(){
    
}

void ReadBoth(){

}
