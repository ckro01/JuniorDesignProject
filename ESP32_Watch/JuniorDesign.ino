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
#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>
#include <BLEBeacon.h>
#include <math.h>
BLEScan *pBLEScan;
#define ENDIAN_CHANGE_U16(x) ((((x)&0xFF00) >> 8) + (((x)&0xFF) << 8))
int scanTime = 1000; //In seconds
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
int d =0;
unsigned long chrono = 0; //Circle variable
bool drawCircle (int centerX, int centerY, int rad, int timeDelay)
{
 if (millis() - chrono < 10) return 0;
 if(timeDelay < 20){
    chrono = millis();
    tft.drawCircle(centerX, centerY, rad, GREEN);
    delay(timeDelay);
    return 1;
  }
  if(timeDelay < 40 && timeDelay >= 20){
    chrono = millis();
    tft.drawCircle(centerX, centerY, rad, YELLOW);
    delay(timeDelay);
    return 1;
  }
  if(timeDelay < 80 && timeDelay >= 40){
    chrono = millis();
    tft.drawCircle(centerX, centerY, rad, WHITE);
    delay(timeDelay);
    return 1;
  }
  if(timeDelay < 120 && timeDelay >= 80){
    chrono = millis();
    tft.drawCircle(centerX, centerY, rad, RED);
    delay(timeDelay);
    return 1;
  }
}

//distance class, do not touch :)
//needs to go at the start, proabably best to call the draw method from here as well.
class MyAdvertisedDeviceCallbacks : public BLEAdvertisedDeviceCallbacks
{
    void onResult(BLEAdvertisedDevice advertisedDevice)
    {
      if (digitalRead(BUTTON_PRESS) == LOW) {
      ESP.restart();
  }
      if (advertisedDevice.haveName())
      {
       std::string devName = advertisedDevice.getName().c_str();
        if (devName.compare("RDL51822") ==0){
          Serial.println("\nTracker Found\n");
          int tmp = advertisedDevice.getRSSI();
          Serial.println(tmp);
          float distanceTemp = 1;
          distanceTemp = (-60 - tmp);
          double dist = pow(10.0,distanceTemp/40);
          Serial.print(dist);
          Serial.print("m");
          d = int(dist);
          display_track();
        }
      }
      

      uint8_t *payLoad = advertisedDevice.getPayload();

      BLEUUID checkUrlUUID = (uint16_t)0xfeaa;
    }
};

void display_track(){
  chrono = millis();
  int radius, circleX = 120, circleY = 120, j=0;    //Setting a d variable to act as the distance variable for when we actually have the bluetooth distance working
  if (d >= 5){         //mulitple functions to display the difference in timing intervals
      radius = 5;
      tft.fillScreen(BLACK);
      while (radius < 100) {
        bool circleDrawn = drawCircle (circleX, circleY, radius, 100);
        if (circleDrawn){
          radius += 20;
        } 
      } 
      soundFour();
      j++;
    j=0;
  }
  else if (d < 5 && d >= 3){
      radius = 5;
      tft.fillScreen(BLACK);
      while (radius < 100) {
        bool circleDrawn = drawCircle (circleX, circleY, radius, 50);
        if (circleDrawn){
          radius += 20;
        } 
      } 
      soundThree();
      j++;
    j=0;
  }
  else if (d < 3 && d >=1){
      radius = 5;
      tft.fillScreen(BLACK);
      while (radius < 100) {
        bool circleDrawn = drawCircle (circleX, circleY, radius, 25);
        if (circleDrawn){
          radius += 20;
        } 
      } 
      soundTwo();
      j++;
    j=0;
  }
  else if (d < 1){
      radius = 5;
      tft.fillScreen(BLACK);
      while (radius < 100) {
        bool circleDrawn = drawCircle (circleX, circleY, radius, 10);
        if (circleDrawn){
          radius += 20;
        } 
      } 
      soundOne();
      j++;
    j=0;
  }
  else{
    tft.fillScreen(BLACK);
    tft.setCursor(100, 120);
    tft.setTextColor(RED);
    tft.println("ERROR");
    tft.setTextColor(BLACK);
  }
}

//initialization
void setup(void) {
  tft.fillScreen(BLACK); //blank screen
  Serial.begin(115200); //init serial
  BLEDevice::init("");
  pBLEScan = BLEDevice::getScan(); //create new scan
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setActiveScan(true); //active scan uses more power, but get results faster
  pBLEScan->setInterval(100);
  pBLEScan->setWindow(99); // less or equal setInterval value
  Serial.println("Scanner Enabled");
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
  colorWipe(strip.Color(0, 0, 255), 50);  // Green
  delay(100); //maybe do something here, otherwise just allows user to read the loading screen
  colorWipe(strip.Color(0, 0, 0), 50);    // Black/off
  //animation screen
  tft.fillScreen(BLACK);
  base_circle(10, BLUE);
  outline_circle(10, WHITE);
  main_menu(); //main menu function
  tft.invertDisplay(true);
  strip.begin();
  strip.show(); // set all pixels to 'off', saves power
}

int i = 0; //loop var
bool flag, select,trackPage,track = false; //flag helps the program know not to update the screen repeatedly unless it is pressed again, select will choose whichever menu is hovered over when triggered
 //program to call when drawing the circle
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
  // Debouncing method, checks for at least 20ms of hold in order to aleviate accidental presses, select wheel main press
  if (newState == LOW && oldState == HIGH) {
    // Short delay to debounce button.
    delay(20);
    newState = digitalRead(BUTTON_PRESS); //reassign new state to compare against old state
    Serial.println(!newState); //debugging
    select = !newState; //assign select bool
    if (select == 1 && !trackPage) { //actual select method
      Serial.println("select triggered"); //debugging
      soundThree(); //new sound
      colorWipe(strip.Color(0, 0, 255), 50);  // Green
      colorWipe(strip.Color(0, 0, 0), 50);    // Black/off 
      delay(100);
      page_select(i); //short delay, then select desired page
      
    }
    else if (select==1 && trackPage) {
      track=true; //enable tracking
      Serial.println("track enabled");
      load_track();
    }
  }
  oldState = newState; //new debouce
  select_main(i, select); //hover function
  delay(20); //short delay
}

void select_main(int pos, int select) {
  if (pos == 0 && !flag) {
    tft.fillScreen(BLACK);
    tft.fillRoundRect(5, 15, 230, 70, 8, CYAN);
    tft.fillRoundRect(10, 20, 220, 60, 8, WHITE);
    tft.setCursor(30, 40);
    tft.setTextColor(BLACK);
    tft.setTextSize(2);
    tft.println("FRISBEE TRACKER");
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
    tft.fillScreen(BLACK);
    tft.fillRoundRect(5, 15, 230, 70, 8, CYAN);
    tft.fillRoundRect(10, 20, 220, 60, 8, WHITE);
    tft.setCursor(30, 40);
    tft.setTextColor(BLACK);
    tft.setTextSize(2);
    tft.println("FRISBEE TRACKER");
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
  tft.fillRoundRect(10, 90, 100, 60, 8, BLUE);
  tft.fillRoundRect(15, 95, 90, 50, 8, WHITE);
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
  //Track page here
  if (!track) {
    tft.fillScreen(BLACK);
    tft.fillRoundRect(5, 15, 230, 70, 8, CYAN);
    tft.fillRoundRect(10, 20, 220, 60, 8, WHITE);
    tft.setCursor(30, 40);
    tft.setTextColor(BLACK);
    tft.setTextSize(2);
    tft.println("FRISBEE TRACKER");
    //set track menu option
    tft.fillRoundRect(5, 100, 230, 70, 8, RED);
    tft.fillRoundRect(10, 105, 220, 60, 8, WHITE);
    tft.setCursor(35, 125);
    tft.setTextColor(GREEN);
    tft.println("Start Tracking");
    trackPage=true;
    Serial.println("Loading Track Page");
  }
  if (track==true){
  tft.fillRoundRect(5, 100, 230, 70, 8, GREEN);
  tft.fillRoundRect(10, 105, 220, 60, 8, WHITE);
  tft.setCursor(35, 125);
  tft.setTextColor(GREEN);
  tft.println("Start Tracking");
  delay(500);
  //load ty stuff
  while(track==true){
  Serial.println("Tracking Started");
  BLEScanResults foundDevices = pBLEScan->start(scanTime, false);
  Serial.println("Scan done!");
  pBLEScan->clearResults(); // delete results fromBLEScan buffer to release memory
  delay(200);
  if (digitalRead(BUTTON_PRESS) == LOW) {
    track==false;
  }
  }
  }
}
void load_about() { 
  //about page here
  Serial.println("Loading About Page");
  tft.fillScreen(BLACK);
  tft.fillRoundRect(5, 15, 230, 70, 8, CYAN);
  tft.fillRoundRect(10, 20, 220, 60, 8, WHITE);
  tft.setCursor(20, 40);
  tft.setTextColor(BLACK);
  tft.setTextSize(2);
  tft.println("About Our Project");
  tft.fillRoundRect(5, 90, 230, 140, 8, MAGENTA);
  tft.fillRoundRect(10, 95, 220, 130, 8, WHITE);
  tft.setCursor(20, 100);
  tft.setTextColor(BLACK);
  tft.setTextSize(1.5);
  tft.println("Our Project is for those who lose");          //For capital letters begin at x = 20, and for lowercase begin at x = 17
  tft.setCursor(17, 115);
  tft.println("their discs too much and could use");
  tft.setCursor(17, 130);
  tft.println("some extra help. The functionality");
  tft.setCursor(17, 145);
  tft.println("of the device is to act as a hot &");
  tft.setCursor(17, 160);
  tft.println("cold form of tracking so that");
  tft.setCursor(20, 175);
  tft.println("players can save time and money");
  tft.setCursor(20, 190);
  tft.println("while enjoying the game of Frolf");
  tft.setCursor(19, 205);
  tft.println("Made By: Ty, Ajay and Caden");
  tft.setTextSize(2);         
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
//default sound #4
void soundFour() {
  unsigned char i;
  for (i = 0; i < 2; i++) {
    for (i = 0; i < 1; i++)
    {
      for (i = 0; i < 12; i++)
      {
        digitalWrite(BUZZER, HIGH);
        delay(1);
        digitalWrite(BUZZER, LOW);
        delay(3);
      }
      for (i = 0; i < 25; i++)
      {
        digitalWrite(BUZZER, HIGH);
        delay(3);
        digitalWrite(BUZZER, LOW);
        delay(1);
      }
    }
  }
}
//rainbowCycle,Wheel, colorWipe are all supplied functions from Neopixel
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

