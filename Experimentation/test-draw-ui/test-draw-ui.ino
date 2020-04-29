#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7789.h> // Hardware-specific library for ST7789
#include <SPI.h>

// Because of the limited number of pins available on the Circuit Playground Boards
// Software SPI is used
#define TFT_CS        0
#define TFT_RST       -1 // Or set to -1 and connect to Arduino RESET pin
#define TFT_DC        1
#define TFT_BACKLIGHT PIN_A3 // Display backlight pin

// You will need to use Adafruit's CircuitPlayground Express Board Definition
// for Gizmos rather than the Arduino version since there are additional SPI
// ports exposed.
#if (SPI_INTERFACES_COUNT == 1)
  SPIClass* spi = &SPI;
#else
  SPIClass* spi = &SPI1;
#endif

// OPTION 1 (recommended) is to use the HARDWARE SPI pins, which are unique
// to each board and not reassignable.
//Adafruit_ST7789 tft = Adafruit_ST7789(spi, TFT_CS, TFT_DC, TFT_RST);

// OPTION 2 lets you interface the display using ANY TWO or THREE PINS,
// tradeoff being that performance is not as fast as hardware SPI above.
#define TFT_MOSI      PIN_A5  // Data out
#define TFT_SCLK      PIN_A4  // Clock out
Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);


void setup(void) {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.print(F("Hello! ST77xx TFT Test"));

  tft.init(240, 240);                // Init ST7789 240x240
  tft.setRotation(2);  
  pinMode(TFT_BACKLIGHT, OUTPUT);
  digitalWrite(TFT_BACKLIGHT, HIGH); // Backlight on

  Serial.println(F("Initialized"));

  uint16_t time = millis();
  tft.fillScreen(ST77XX_BLACK);
  time = millis() - time;

  Serial.println(time, DEC);
  delay(500);

  tft.fillScreen(ST77XX_WHITE);
  tft.setTextSize(10);
  testdrawtext("1", ST77XX_BLACK);
  delay(500);
  tft.fillScreen(ST77XX_BLACK);
  tft.setTextSize(9);
  testdrawtext("2", ST77XX_WHITE);
  delay(500);
  tft.fillScreen(ST77XX_MAGENTA);
  tft.setTextSize(8);
  testdrawtext("3", ST77XX_BLACK);
  delay(500);
  tft.fillScreen(ST77XX_WHITE);
  tft.setTextSize(7);
  testdrawtext("4", ST77XX_BLACK);
  delay(500);
  tft.fillScreen(ST77XX_RED);
  tft.setTextSize(6);
  testdrawtext("5", ST77XX_BLACK);
  delay(500);
    tft.fillScreen(ST77XX_ORANGE);
  tft.setTextSize(5);
  testdrawtext("6", ST77XX_BLACK);
  delay(500);
    tft.fillScreen(ST77XX_WHITE);
  tft.setTextSize(4);
  testdrawtext("7", ST77XX_BLACK);
  delay(500);
    tft.fillScreen(ST77XX_YELLOW);
  tft.setTextSize(5);
  testdrawtext("8", ST77XX_BLACK);
  delay(500);
  tft.setTextSize(6);
  testdrawtext("9", ST77XX_BLACK);
  tft.fillScreen(ST77XX_GREEN);
  delay(500);
    tft.fillScreen(ST77XX_YELLOW);
  tft.setTextSize(7);
  testdrawtext("10", ST77XX_BLACK);
  delay(500);
  tft.setTextSize(8);
  testdrawtext("11", ST77XX_BLACK);
  tft.fillScreen(ST77XX_BLUE);
  delay(500);
  tft.setTextSize(9);
  testdrawtext("12", ST77XX_BLACK);
  tft.fillScreen(ST77XX_GREEN);
  delay(500);
  tft.setTextSize(10);
  testdrawtext("13", ST77XX_BLACK);
  tft.fillScreen(ST77XX_CYAN);
  delay(500);
  tft.setTextSize(11);
  testdrawtext("14", ST77XX_BLACK);
  tft.fillScreen(ST77XX_MAGENTA);
  delay(500);
  tft.setTextSize(12);
  testdrawtext("15", ST77XX_BLACK);
  tft.fillScreen(ST77XX_MAGENTA);
  delay(500);
  tft.setTextSize(13);
  testdrawtext("16", ST77XX_BLACK);
  tft.fillScreen(ST77XX_MAGENTA);
  delay(500);
  tft.fillScreen(ST77XX_MAGENTA);
    tft.setTextSize(20);
  testdrawtext("20", ST77XX_BLACK);
    delay(1000);
  // large block of text
  //tft.setCursor(0, 0);
  tft.fillScreen(ST77XX_BLACK); //have to reset the screen everytime or it will draw over previous
  tft.setTextSize(1);
  testdrawtext("Welcome! Please Assemble your pizza ☆☆☆☆☆☆ \u2605 ", ST77XX_WHITE);
  delay(1000);
  tft.fillScreen(ST77XX_BLACK);
  
  tft.setTextSize(2);
  testdrawtext("Press button A when complete ", ST77XX_WHITE);
  delay(1000);
  tft.fillScreen(ST77XX_BLACK);

  tft.setTextSize(3);
  testdrawtext("Your pizza has: cheese, tomato, mushroom, salami, red pepper, green pepper ", ST77XX_WHITE);
  delay(1000);
  tft.fillScreen(ST77XX_BLACK);

  tft.setTextSize(4);
  testdrawtext("Suggested swap: Salami ---> Ham \n \t Salami has more salt than ham.", ST77XX_WHITE);
  delay(1000);

  tft.setTextSize(5);
  tft.fillScreen(ST77XX_BLACK);
  testdrawtext("Would you like to order this pizza? \n\n\n\n y/n  ", ST77XX_BLUE);
  delay(1000);

  tft.setTextSize(1);
  tft.fillScreen(ST77XX_GREEN);
  testdrawtext(" Pizza Ordered!!  ", ST77XX_RED);
  delay(1000);
  digitalWrite(TFT_BACKLIGHT, LOW); 
}

void testdrawtext(char *text, uint16_t color) {
  tft.setCursor(0, 0);
  tft.setTextColor(color);
  tft.setTextWrap(true);
  tft.print(text);
}

void loop() {
  // put your main code here, to run repeatedly:

}
