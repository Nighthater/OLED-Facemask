//----------------------------------------------------------------------------------------
// DEFINITIONS
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

#define i2c_Address 0x3c //initialize with the I2C addr 0x3C Typically eBay OLED's

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET -1   //   QT-PY / XIAO
Adafruit_SH1106G display = Adafruit_SH1106G(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// BUTTON DEFINITIONS
// PINS IN ORDER ARE D0, D2, D3, D4, D5, D6, D7, D10/TX, D9/RX
#define FACE_ACTION_01 16
#define FACE_ACTION_02 0
#define FACE_ACTION_03 2
#define FACE_ACTION_04 14
#define FACE_ACTION_05 12
#define FACE_ACTION_06 13
#define FACE_ACTION_07 1
#define FACE_ACTION_RESET 3

// TIMING FOR BLINKING
unsigned long myTime;
unsigned long loopTime;
unsigned long blinkTime;

//----------------------------------------------------------------------------------------
// SETUP
void setup()   {
  delay(250); // wait for the OLED to power up
  display.begin(i2c_Address, true); // Address 0x3C default
  display.display();
  display.clearDisplay();

  // PINS IN ORDER ARE D0, D2, D3, D4, D5, D6, D7, D10/TX, D9/RX
  pinMode(FACE_ACTION_01, INPUT); 
  pinMode(FACE_ACTION_02, INPUT);
  pinMode(FACE_ACTION_03, INPUT);
  pinMode(FACE_ACTION_04, INPUT);
  pinMode(FACE_ACTION_05, INPUT);
  pinMode(FACE_ACTION_06, INPUT);
  pinMode(FACE_ACTION_07, INPUT);
  pinMode(FACE_ACTION_RESET, INPUT);
  //Serial.begin(9600);
}

//----------------------------------------------------------------------------------------
// LOOP
void loop() {
  loopTime = millis() - myTime;
  myTime = millis();
  
  // LOOK AT PRESSED BUTTONS AND START ANIMATION UNTIL RESET BUTTON IS PRESSED
  if(digitalRead(FACE_ACTION_01) == LOW) anim_Face_V_UP();
    else if(digitalRead(FACE_ACTION_02) == LOW) anim_Face_Scanning();
      else if(digitalRead(FACE_ACTION_03) == LOW) drawFace_V_RIGHT();
        else if(digitalRead(FACE_ACTION_04) == LOW) anim_Face_Static();
          else if(digitalRead(FACE_ACTION_05) == LOW) anim_Face_Pupil_dilate();
            else if(digitalRead(FACE_ACTION_06) == LOW) anim_Face_Blink_FAST();
              else if(digitalRead(FACE_ACTION_07) == LOW) anim_Face_X();
  else{
    drawFace_O();
    blinkTime = blinkTime + loopTime;
    if(blinkTime >= random(5000,15000)) {
      //blinking
      blinkTime = 0;
      anim_Face_Blink();
    }
  }
  delay(10);
  
}


//----------------------------------------------------------------------------------------
// ANIMATION FUNCTIONS
void anim_Face_V_UP(void) {
  anim_TR_O_to_Dot();
  display.clearDisplay();
  for(int i = 0; i<=32;i = i + 4){
    display.clearDisplay();
    display.fillTriangle(64-i, 64, 64, 0, 64+i, 64, SH110X_WHITE);
    display.fillTriangle(64-(i*0.75), 64, 64, 16, 64+(i*0.75), 64, SH110X_BLACK);
    display.display();
  }
  delay(200);
  while(digitalRead(FACE_ACTION_RESET)==HIGH){
    delay(50);
  }
  for(int i = 32; i>=0;i = i - 4){
    display.clearDisplay();
    display.fillTriangle(64-i, 64, 64, 0, 64+i, 64, SH110X_WHITE);
    display.fillTriangle(64-(i*0.75), 64, 64, 16, 64+(i*0.75), 64, SH110X_BLACK);
    display.display();
  }
  anim_TR_Dot_to_O();
  delay(200);
  display.display();

}



void anim_Face_Scanning(void) {
  anim_TR_O_to_Dot();
  display.clearDisplay();
  float angle = 0.0;
  while(digitalRead(FACE_ACTION_RESET)==HIGH){
    display.clearDisplay();
    angle = angle + 0.05;
    display.drawFastHLine(0,(sin(angle*2)+1)*32,128,SH110X_WHITE);
    display.drawFastVLine((sin(angle)+1)*64,0,64,SH110X_WHITE);
    display.display();
  }
  display.clearDisplay();
  anim_TR_Dot_to_O();
}



void drawFace_V_RIGHT(void) {
  //DISPLAY LEFT EYE
  display.clearDisplay();
  display.fillTriangle(0, 0, 128, 32, 0, 64, SH110X_WHITE);
  display.fillTriangle(0, 8, 96, 32, 0, 56, SH110X_BLACK);
  display.display();
  //DISPLAY RIGHT EYE  
}

void anim_Face_X(void) {
  display.clearDisplay();
  display.fillScreen(SH110X_WHITE);
  delay(500);
  for(int i = 0; i<=28;i = i + 2){
    display.fillScreen(SH110X_WHITE);
    display.fillTriangle(4, 64, 124, 64, 64, 64-i, SH110X_BLACK);
    display.fillTriangle(4, 0, 124, 0, 64, i, SH110X_BLACK);
    display.fillTriangle(0, 4, 0, 60, i*2, 32, SH110X_BLACK);
    display.fillTriangle(128, 4, 128, 60, 128-i*2, 32, SH110X_BLACK);
    display.display();
  }
  while(digitalRead(FACE_ACTION_RESET)==HIGH){
    delay(500);
    display.invertDisplay(true);
    delay(500);
    display.invertDisplay(false);
  }
  display.invertDisplay(false);
  display.clearDisplay();
  display.display();
  delay(500);
  // MAKE NORMAL
  for(int i = 0; i<=31; i++)
  {
    display.clearDisplay();
    display.fillCircle(64, 31, i, SH110X_WHITE);
    display.fillCircle(64, 31, i-8, SH110X_BLACK);
    display.display();
  }
  delay(50);
}

void anim_Face_Blink(void) {
  drawFace_ALMOSTOPEN();
  delay(30);
  drawFace_SEMICLOSE();
  delay(40);
  drawFace_ALMOSTCLOSED();
  delay(20);
  drawFace_DASH();
  delay(150);
  drawFace_ALMOSTCLOSED();
  delay(20);
  drawFace_SEMICLOSE();
  delay(40);
  drawFace_ALMOSTOPEN();
  delay(30);
}

void anim_Face_Blink_FAST(void) {
  drawFace_ALMOSTOPEN();
  delay(20);
  drawFace_SEMICLOSE();
  delay(15);
  drawFace_ALMOSTCLOSED();
  delay(10);
  drawFace_DASH();
  delay(50);
  drawFace_ALMOSTCLOSED();
  delay(10);
  drawFace_SEMICLOSE();
  delay(15);
  drawFace_ALMOSTOPEN();
  delay(20);
  blinkTime = 0;
}

void anim_Face_Pupil_dilate(void) {
  delay(20);
  //MAKE SMALL
  for(int i = 23; i>=0; i = i - 4)
  {
    display.clearDisplay();
    display.fillCircle(64, 31, 31, SH110X_WHITE);
    display.fillCircle(64, 31, i, SH110X_BLACK);
    display.display();
  }
  for(int i = 31; i>=3; i = i - 4)
  {
    display.clearDisplay();
    display.fillCircle(64, 31, i, SH110X_WHITE);
    display.display();
  }
  delay(250);

  //WAIT FOR BUTTON

  while(digitalRead(FACE_ACTION_RESET)==HIGH){
    delay(25);
  }
  
  // MAKE NORMAL
  anim_TR_Dot_to_O();
  delay(50);
}

void anim_Face_Static(void) {
  anim_TR_O_to_Dot();
  int loop_counter_int = 1;
  while(loop_counter_int<=20){
    display.clearDisplay();
    for(int i = 0; i<=loop_counter_int;i++){
      
      display.drawFastVLine(random(0,128),0,64,SH110X_WHITE);
      
    }
    display.display();
    delay(30);
    loop_counter_int = loop_counter_int + 1;
  }



  
  while(digitalRead(FACE_ACTION_RESET)==HIGH){
    display.clearDisplay();
    for(int i = 1; i<20; i = i + 1)
    {
      //display.drawPixel(random(1,123), random(1,63), SH110X_WHITE);
      display.drawFastVLine(random(0,128),0,64,SH110X_WHITE);
    }
    display.display();
    //delay(30);
  }
  loop_counter_int = 20;
  while(loop_counter_int>=1){
    display.clearDisplay();
    for(int i = 0; i<=loop_counter_int;i++){
      
      display.drawFastVLine(random(0,128),0,64,SH110X_WHITE);
      
    }
    display.display();
    delay(30);
    loop_counter_int = loop_counter_int - 1;
  }
  display.clearDisplay();
  display.drawPixel(64, 32, SH110X_WHITE);
  display.display();
  delay(1000);
  // MAKE NORMAL
  anim_TR_Dot_to_O();
  delay(50);
}

//----------------------------------------------------------------------------------------
// HELPER FUNCTIONS

void anim_TR_O_to_Dot(void){
  for(int i = 31; i>=0; i = i - 4)
  {
    display.clearDisplay();
    display.fillCircle(64, 31, i, SH110X_WHITE);
    display.fillCircle(64, 31, i-8, SH110X_BLACK);
    display.display();
  }
}

void anim_TR_Dot_to_O(void){
  for(int i = 0; i<=31; i = i + 4)
  {
    display.clearDisplay();
    display.fillCircle(64, 31, i, SH110X_WHITE);
    display.fillCircle(64, 31, i-8, SH110X_BLACK);
    display.display();
  }
}

void drawFace_O(void) {
  //DISPLAY LEFT EYE
  display.clearDisplay();
  display.fillCircle(64, 31, 31, SH110X_WHITE);
  display.fillCircle(64, 31, 23, SH110X_BLACK);
  display.display();
  //DISPLAY RIGHT EYE  
}

void drawFace_ALMOSTOPEN(void) {
  //DISPLAY LEFT EYE
  display.clearDisplay();
  display.fillRoundRect(32, 4, 64, 56, 25, SH110X_WHITE);
  display.fillRoundRect(40, 12, 48, 40, 15,  SH110X_BLACK);
  display.display();
  //DISPLAY RIGHT EYE  
}

void drawFace_SEMICLOSE(void) {
  //DISPLAY LEFT EYE
  display.clearDisplay();
  display.fillRoundRect(16, 8, 80, 40, 20, SH110X_WHITE);
  display.fillRoundRect(24, 16, 64, 24, 10,  SH110X_BLACK);
  display.display();
  //DISPLAY RIGHT EYE  
}

void drawFace_ALMOSTCLOSED(void) {
  //DISPLAY LEFT EYE
  display.clearDisplay();
  display.fillRoundRect(8, 16, 112, 32, 12, SH110X_WHITE);
  display.fillRoundRect(16, 28, 96, 8, 4,  SH110X_BLACK);
  display.display();
  //DISPLAY RIGHT EYE  
}

void drawFace_DASH(void) {
  //DISPLAY LEFT EYE
  display.clearDisplay();
  display.fillRect(0, 28, 128, 8, SH110X_WHITE);
  display.display();
  //DISPLAY RIGHT EYE  
}
