//import libraries
#include <SPI.h> //Circuit playground uses SPI to receive image data
#include <SparkFun_UHF_RFID_Reader.h> //RFID reader library
#include <Vector.h>                   // Storage handling


//other 
// ==== supporting routines
void serialTrigger(String message);

#define EPC_COUNT 50        // Max number of unique EPC's
#define EPC_ENTRY 12        // max bytes in EPC
#define HTML_BUFFER 400     // Size of HTML buffer

typedef struct Epcrecv {
    uint8_t epc[EPC_ENTRY];         // Save EPC
    uint8_t cnt;                    // How often detected
} Epcrecv;

Epcrecv epc_space[EPC_COUNT];       // Allocate the space
Vector <Epcrecv> epcs;              // Create vector



//Define RFID shield pins
#include <SoftwareSerial.h> //For transmitting to digital pins. https://www.arduino.cc/en/Reference/softwareSerial
SoftwareSerial softSerial(2, 3); //RX, TX
  /* Option: define the serial speed to communicate with the shield */
#define RFID_SERIAL_SPEED 38400

 /* MUST: Define the region in the world you are in
  * It defines the frequency to use that is available in your part of the world
  *
  * Valid options are :
  * REGION_INDIA, REGION_JAPAN, REGION_CHINA, REGION_EUROPE, REGION_KOREA,
  * REGION_AUSTRALIA, REGION_NEWZEALAND, REGION_NORTHAMERICA
  */
#define RFID_Region REGION_EUROPE

  /* Option: define the Readpower between 0 and 27dBm. if only using the
   * USB power, do not go above 5dbm (=500) */
#define RFID_POWER 500 //Lukes code had 2700, using 500 to be safe for now

 /* Option: display debug level/information
  * 0 = no debug information
  * 1 = tag info found
  * 2 = 1 + programflow + temperature info
  * 3 = 2 + Nano debug  */
#define PRMDEBUG 2
unsigned long period = 30000; // Variables to control how long we check for new tags
RFID nano; //instantiate an instance of nano 
#define SERIAL 115200 // Defines the baud rate for communication over the serial port - (bits per second communicated)

//send initial hello to output - console first, then CP Gizmo
void setup(){
  Serial.print("Code begins...");
  //connectToPlayground();
  Serial.begin(SERIAL);
  Init_RFID();        // initialize RFID shield
  Array_Init();       // initialize array
 }

void loop(){
  
  
}

//RFID stuff
void Init_RFID()
{

retry:
  Serial.println(F("try to connect to RFID reader"));

  // Configure Nano to run at certain speed
  int RunStatus = setupNano(RFID_SERIAL_SPEED);

  if (RunStatus == 0)
  {
    serialTrigger(F("Module failed to respond. Please check wiring."));
    yield();
    goto retry;
  }

  if (RunStatus  == 1)           // was not yet in continuoos mode
  {
    nano.setRegion(RFID_Region); // Set to correct region
    nano.setTagProtocol();       // Set protocol to GEN2
    nano.setAntennaPort();       // Set TX/RX antenna ports to 1
    nano.setReadPower(RFID_POWER); //5.00 dBm. Higher values may caues USB port to brown out
    //Max Read TX Power is 27.00 dBm and may cause temperature-limit throttling

    nano.startReading();         // Begin scanning for tags
  }

  Serial.println("RFID shield initialized");
}

/* Gracefully handles a reader that is already configured and already reading continuously */
int setupNano(long baudRate)
{
  if (PRMDEBUG == 3) nano.enableDebugging(Serial);

  nano.begin(softSerial); //Tell the library to communicate over software serial port

  //Test to see if we are already connected to a module
  //This would be the case if the Arduino has been reprogrammed/rebooted and the module has stayed powered
  softSerial.begin(baudRate);       // For this test, assume module is already at our desired baud rate

  uint8_t val1 = 0;

  while(val1 < 2)
  {
    nano.getVersion();

    if (nano.msg[0] == ERROR_WRONG_OPCODE_RESPONSE )
    {
      if (PRMDEBUG > 1) Serial.println(F("Module continuously reading. Not Asking it to stop..."));
      return(2);
    }

    else if (nano.msg[0] != ALL_GOOD)
    {
      if (PRMDEBUG > 1) Serial.println(F("Try reset RFID speed"));

      // The module did not respond so assume it's just been powered on and communicating at 115200bps
      softSerial.begin(115200);    // Start software serial at 115200

      nano.setBaud(baudRate);   // Tell the module to go to the chosen baud rate.

      softSerial.begin(baudRate);  // Start the software serial port, this time at user's chosen baud rate
      delay(1000);              // the driver is normally not waiting for response from the Nano. So we need to delay and wait
    }
    else
      return(1);                // Responded, but have to be set in the right mode

    val1++;
  }

  return(0);
}

/* try to read EPC from tag and add to array */
void Check_EPC()
{
  uint8_t myEPC[EPC_ENTRY];     // Most EPCs are 12 bytes

  if (nano.check() == true)     // Check to see if any new data has come in from module
  {
    byte responseType = nano.parseResponse(); //Break response into tag ID, RSSI, frequency, and timestamp

    if (responseType == RESPONSE_IS_KEEPALIVE)
    {
      if (PRMDEBUG > 1) Serial.println(F("Scanning"));
    }
    else if (responseType == RESPONSE_IS_TAGFOUND)
    {
      // extract EPC
      for (byte x = 0 ; x < EPC_ENTRY ; x++)   myEPC[x] = nano.msg[31 + x];

      // add to array (if not already there)
      Array_Add(myEPC, EPC_ENTRY);

      // display for debug information
      if (PRMDEBUG > 0)
      {
        int rssi = nano.getTagRSSI();            // Get the RSSI for this tag read
        long freq = nano.getTagFreq();           // Get the frequency this tag was detected at
        long timeStamp = nano.getTagTimestamp(); // Get the time this was read, (ms) since last keep-alive message

        Serial.print(F(" rssi["));
        Serial.print(rssi);
        Serial.print(F("] freq["));

        Serial.print(freq);
        Serial.print(F("] time["));

        Serial.print(timeStamp);
        Serial.print(F("] epc["));

        // Print EPC bytes,
        for (byte x = 0 ; x < EPC_ENTRY ; x++)
        {
          if (myEPC[x] < 0x10) Serial.print(F("0")); // Pretty print adding zero
          Serial.print(myEPC[x], HEX);
          Serial.print(F(" "));
        }
        Serial.println("]");
      }
    }
    else                                             // Unknown response
    {
      if (PRMDEBUG > 1) nano.printMessageArray();    // Print the response message. Look up errors in tmr__status_8h.html
    }
  }
}
//ARRAY routines


/* initialize the array to capture detected EPC */
void Array_Init()
{
  epcs.setStorage(epc_space);
}

/* count number of entries with unique EPC received */
int Array_Cnt()
{
  return((int) epcs.size());
}

/* Empty the EPC array */
void Array_Clr()
{
  epcs.clear();
}

// add entry to array (if not there already
void Array_Add(uint8_t *msg, byte mlength)
{
    bool found = false;
    byte j;
    size_t i;

    for (i = 0; i < epcs.size(); i++)
    {
        found = true;

        // check each entry for a match
        for (j = 0 ; j < mlength, j < EPC_ENTRY ; j++)
        {
          if (epcs[i].epc[j] != msg[j])
          {
            found = false;  // indicate not found
            break;
          }
        }

        if (found)
        {
          if (PRMDEBUG > 1)
          {
            Serial.print(F("FOUND EPC in array entry "));
            Serial.println(i);
          }
          epcs[i].cnt++;
          return;
        }
    }

  // debug info
  if (PRMDEBUG > 1) Serial.println(F("Not found adding"));

  if ( i >= epcs.max_size())
  {
    Serial.println(F("Can not add more"));
    return;
  }

  // add new entry in array
  Epcrecv newepc;
  for (j = 0 ; j < EPC_ENTRY ; j++) newepc.epc[j] = msg[j];
  newepc.cnt = 0;
  epcs.push_back(newepc);     // append to end
}

//Supporting routines

/* serialTrigger prints a message, then waits for something
 * to come in from the serial port. */
void serialTrigger(String message) //understand is this needed or is this just fancy consolelog - change error catching
{
  Serial.println();
  Serial.println(message);
  Serial.println();
  while (!Serial.available());
  while (Serial.available())Serial.read();
}

//
//void connectToPlaygound(){
  //connect via Wifi.
  //connect via bluetooth?
  //gizmo says hello
//}
