//Junior Design ESP32 Watch Code, Caden Krohn, Ty Lancaster, Aswathy Ajay
//include libraries
#include <Adafruit_Crickit.h>
#include <Adafruit_miniTFTWing.h>
#include <Adafruit_NeoKey_1x4.h>
#include <Adafruit_NeoTrellis.h>
#include <Adafruit_TFTShield18.h>
#include <Adafruit_GFX.h>    // Core graphics library by Adafruit
#include <Arduino_ST7789.h> // Hardware-specific library for ST7789 (with or without CS pin)
#include <Adafruit_NeoPixel.h>
#include <SPI.h>
//pin definitions on watch
#define BUTTON_PRESS   18
#define BUTTON_LEFT    5
#define BUTTON_RIGHT   19
#define PIXEL_PIN    33    // Digital IO pin connected to the NeoPixels.
#define PIXEL_COUNT 1
#define BUZZER 32
#define TFT_DC    22
#define TFT_RST   23
#define TFT_MOSI  17   // for hardware SPI data pin (all of available pins)
#define TFT_SCLK  16   // for hardware SPI sclk pin (all of available pins)
//screen setup
Adafruit_NeoPixel strip = Adafruit_NeoPixel(PIXEL_COUNT, PIXEL_PIN, NEO_GRB + NEO_KHZ800);
Arduino_ST7789 tft = Arduino_ST7789(TFT_DC, TFT_RST, TFT_MOSI, TFT_SCLK); //for display without CS pin
bool oldState = HIGH; //debouce bool
//initialization
void setup(void) {
  tft.fillScreen(BLACK); //blank screen
  Serial.begin(115200); //init serial
  //init buttons + buzzer
  pinMode(BUTTON_PRESS, INPUT_PULLUP);
  pinMode(BUTTON_LEFT, INPUT_PULLUP);
  pinMode(BUTTON_RIGHT, INPUT_PULLUP);
  pinMode(BUZZER, OUTPUT);
  //init screen
  tft.init(240, 240);   // initialize a ST7789 chip, 240x240 pixels
  Serial.println("Initialized");
  uint16_t time = millis(); //time variable if timer wanted later
  //loading screen
  tft.fillScreen(BLACK);
  delay(500);
  tft.fillScreen(BLACK);
  tft.setCursor(60, 120);
  tft.setTextColor(WHITE);
  tft.setTextSize(3);
  tft.setTextWrap(true);
  tft.println("LOADING");
  delay(100); //maybe do something here, otherwise just allows user to read the loading screen
  rainbowCycle(2);
  //animation screen
  tft.fillScreen(BLACK);
  base_circle(10, BLUE);
  outline_circle(10, WHITE);
  main_menu(); //main menu function
  delay(500);
  tft.invertDisplay(true);
  delay(500);
  strip.begin();
  strip.show(); // set all pixels to 'off', saves power
}
int i = 0; //loop var
bool flag, select = false; //flag helps the program know not to update the screen repeatedly unless it is pressed again, select will choose whichever menu is hovered over when triggered
void loop() { //main program loop
  bool newState = digitalRead(BUTTON_PRESS); //deboucing new state
  if (digitalRead(BUTTON_LEFT) == LOW) //moving left
  {
    if (i < 0) {
      i = 1;
      flag = false; //reset flag, decrement i, handle if exceeds menu options
    } else {
      i--;
    }
  }
  if (digitalRead(BUTTON_RIGHT) == LOW) //moving right
  { //i increments and decrements with the scroll wheel, resets when out of bounds, flag gets reset when i is changed, otherwise remains true to avoid flashing.
    if (i > 1) {
      i = 0;
      flag = false;
    } else {
      i++;
    }
  }
  // Deboucing method, checks for at least 20ms of hold in order to aleviate accidental presses, select wheel main press
  if (newState == LOW && oldState == HIGH) {
    // Short delay to debounce button.
    delay(20);
    newState = digitalRead(BUTTON_PRESS); //reassign new state to compare against old state
    Serial.println(!newState); //debugging
    select = !newState; //assign select bool
    if (select == 1) { //actual select method
      Serial.println("select triggered"); //debugging
      soundThree(); //new sound
      colorWipe(strip.Color(0, 0, 255), 50);  // Green
    colorWipe(strip.Color(0, 0, 0), 50);    // Black/off 
      delay(100);
      page_select(i); //short delay, then select desired page
    }
  }
  oldState = newState; //new debouce
  select_main(i, select); //hover function
  delay(20); //short delay
}
void select_main(int pos, int select) {
  if (pos == 0 && !flag) {
    Serial.println("User Hovering Above Track");
    soundOne(); //move left sound
    colorWipe(strip.Color(0, 255, 0), 50);  // Green
    colorWipe(strip.Color(0, 0, 0), 50);    // Black/off 
    //update track block to highlight blue, resets the other block just in case it was previously hovered
    tft.fillRoundRect(10, 90, 100, 60, 8, BLUE);
    tft.fillRoundRect(15, 95, 90, 50, 8, WHITE);
    tft.setCursor(30, 110);
    tft.setTextColor(GREEN);
    tft.println("Track");
    tft.fillRoundRect(10, 160, 100, 60, 8, WHITE);
    tft.setCursor(30, 180);
    tft.setTextColor(RED);
    tft.println("About");
    flag = true; //trigger flag so it doesn't continously select main, otherwise it will flash uncontrollably 
  }
  else if (pos == 1 && !flag) {
    Serial.println("User Hovering Above About");
    soundTwo(); //move right sound
    colorWipe(strip.Color(0, 255, 0), 50);  // Green
    colorWipe(strip.Color(0, 0, 0), 50);    // Black/off 
    //update about block to highlight blue, resets the other block just in case it was previously hovered
    tft.fillRoundRect(10, 90, 100, 60, 8, WHITE);
    tft.setCursor(30, 110);
    tft.setTextColor(GREEN);
    tft.println("Track");
    tft.fillRoundRect(10, 160, 100, 60, 8, BLUE);
    tft.fillRoundRect(15, 165, 90, 50, 8, WHITE);
    tft.setCursor(30, 180);
    tft.setTextColor(RED);
    tft.println("About");
    flag = true; //trigger flag so it doesn't continously select main, otherwise it will flash uncontrollably 
  }
}
void main_menu() {
  //main menu initialize
  //set main title block
  tft.fillScreen(BLACK);
  tft.fillRoundRect(5, 15, 230, 70, 8, CYAN);
  tft.fillRoundRect(10, 20, 220, 60, 8, WHITE);
  tft.setCursor(30, 40);
  tft.setTextColor(BLACK);
  tft.setTextSize(2);
  tft.println("FRISBEE TRACKER");
  //set track menu option
  tft.fillRoundRect(10, 90, 100, 60, 8, WHITE);
  tft.setCursor(30, 110);
  tft.setTextColor(GREEN);
  tft.println("Track");
  //set about menu option
  tft.fillRoundRect(10, 160, 100, 60, 8, WHITE);
  tft.setCursor(30, 180);
  tft.setTextColor(RED);
  tft.println("About");
}
//backend menu
void page_select(int i) {
  if (i == 0) {
    load_track();
  }
  else if (i == 1) {
    load_about();
  }
  else {
    Serial.println("uh oh");
    main_menu();
  }
}
void load_track() {
  //track page here, placeholder code
  Serial.println("Loading Track Page");
  main_menu(); //update to know it selected something, will isntead move the user to the track screen
}
void load_about() {
  //about page here
  Serial.println("Loading About Page");
  main_menu(); //update to know it selected something, will instead move the user to the about screen
}
//default sound #1
void soundOne() {
  unsigned char i;
  for (i = 0; i < 1; i++)
  {
    for (i = 0; i < 50; i++)
    {
      digitalWrite(BUZZER, HIGH);
      delay(1);
      digitalWrite(BUZZER, LOW);
      delay(1);
    }
    for (i = 0; i < 25; i++)
    {
      digitalWrite(BUZZER, HIGH);
      delay(1);
      digitalWrite(BUZZER, LOW);
      delay(1);
    }
  }
}
//default sound #2
void soundTwo() {
  unsigned char i;
  for (i = 0; i < 1; i++)
  {
    for (i = 0; i < 25; i++)
    {
      digitalWrite(BUZZER, HIGH);
      delay(1);
      digitalWrite(BUZZER, LOW);
      delay(1);
    }
    for (i = 0; i < 50; i++)
    {
      digitalWrite(BUZZER, HIGH);
      delay(1);
      digitalWrite(BUZZER, LOW);
      delay(1);
    }
  }
}
//default sound #3
void soundThree() {
  unsigned char i;
  for (i = 0; i < 2; i++) {
    for (i = 0; i < 1; i++)
    {
      for (i = 0; i < 12; i++)
      {
        digitalWrite(BUZZER, HIGH);
        delay(1);
        digitalWrite(BUZZER, LOW);
        delay(2);
      }
      for (i = 0; i < 25; i++)
      {
        digitalWrite(BUZZER, HIGH);
        delay(2);
        digitalWrite(BUZZER, LOW);
        delay(1);
      }
    }
  }
}
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
  colorWipe(strip.Color(0,0,0),50); //off
}
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}
//default TFT GFX library code
void base_circle(uint8_t radius, uint16_t color) {
  for (int16_t x = radius; x < tft.width(); x += radius * 2) {
    for (int16_t y = radius; y < tft.height(); y += radius * 2) {
      tft.fillCircle(x, y, radius, color);
    }
  }
}
void outline_circle(uint8_t radius, uint16_t color) {
  for (int16_t x = 0; x < tft.width() + radius; x += radius * 2) {
    for (int16_t y = 0; y < tft.height() + radius; y += radius * 2) {
      tft.drawCircle(x, y, radius, color);
    }
  }
}
