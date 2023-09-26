/*
tft test
*/

#include <SPI.h>
#include "Adafruit_RA8875.h"
#include "Adafruit_GFX.h"

// define intr
// intr allows the progrom to continue running/looping while waiting for events
// as in the case of waiting for touch events on the tft
#define intr 3

// define chip select
//  SPI uses CS pin to have isolated and unique devices
#define chipSelect 10

// reset pin brings the VIN down to LOW effectively shutting ogg power and resetting device
#define reset 9

// digital 3, 9, 10 are used for the auxilliary pins
// the remaining pins used will be on ICSP 1, which is the group of 6 pins near back of UNO or in middle of larger board like DUE
// ICSP in-curcuit Serial Programming

// initialize the adafruit driver board library
// first arg is the chip select
// second arg is intr
// uint8_t means 8 bit number 0-255
Adafruit_RA8875 tftBoard = Adafruit_RA8875(chipSelect, intr);

// analog pins that will read analog signal from a0 and a1
// 3.3 volt

int xPin = A0;
int yPin = A1;
int xPos = 0;
int yPos = 0;

// declare useful functions here and then call them and write them wherever
void pwmTest();
void drawXBar(int xVal);
void drawYBar(int yVal);

void setup()
{

  Serial.begin(9600);
  Serial.println("Start serial");

  // initialze using .begin method or class variable??
  if (!tftBoard.begin(RA8875_800x480))
  {
    // begin method of the Adafruit_RA8875 class is responbsible for verifying appropriate screen size
    // is selected, setting the pinMode of the CS and Int, setting the serial clack speed, check for serial
    // transaction, pinging register (0)
    Serial.println("Failed to Initialize driver board");
    // keep looping until this initializes
    while (1)
      ;
  }
  // else
  Serial.println("TFT Initialized");

  // additional setup after tft initialized
  tftBoard.displayOn(true);
  tftBoard.GPIOX(true);
  // display on will write to the regsiter 0x01 and the BITWISE OR will take 0x00 RA8875_PWRR_NORMAL and 0x80 RA8875_PWRR_DISPON
  //  the result is 0x80
  //     0000 0000
  //     1000 0000
  //-------------
  //     1000 0000
  // writeReg(0x01, 0x00 | 0x80);
  uint8_t pwrr_reg = tftBoard.readReg(RA8875_PWRR);
  Serial.println(pwrr_reg);
  // neat so that does work, only bit 7 is populated == 128

  // setup pwm clk division for 1024::10 bit
  tftBoard.PWM1config(true, RA8875_PWM_CLK_DIV1024);
  // even though you still can only give pwm duty cycle max of 255...
  tftBoard.PWM1out(255);

  // flash screen with white
  tftBoard.fillScreen(RA8875_WHITE);
  pwmTest();
  // Serial.println("Post pwm test");

  //after pwm test reset screen
  tftBoard.PWM1out(255);
  tftBoard.fillScreen(RA8875_BLACK);
  tftBoard.textMode();

  // set the intr pin to high
  // meaning this connects int pin to pullup resistor
  // when intr is connect to PU, it reduces noise
  // and sets a stable state to detect intr events like touching the screen
  pinMode(intr, INPUT);
  digitalWrite(intr, HIGH);
}

void loop()
{
  xPos = analogRead(xPin);
  yPos = analogRead(yPin);

  // Serial.print("X: ");
  // Serial.print(xPos);
  // Serial.print(" | yPos: ");
  // Serial.println(yPos);
  // delay(2000);

  // update the bar graph
  drawXBar(xPos);
  drawYBar(yPos);
  
  delay(100);
};

// void func means it doesn't return any value
void drawXBar(int xVal){
  // takes argument xVal which should be analog 0-1023 bit signal

  // bar width will be screen width but divided into 10 bit division
  // convert 10 bit to width of screen
  // resting position of pot is 511, so convert that to half width of screen
  int barWidth = map(xVal, 0, 1023, 0, tftBoard.width());

  // clear previous rectangle everytime function is called
  tftBoard.fillRect(0, tftBoard.height()*.25, tftBoard.width(), 20, RA8875_BLACK);
  
  // update bar graph
  tftBoard.fillRect(0, tftBoard.height()*.25, barWidth, 20, RA8875_GREEN);

};

// yBar
void drawYBar(int yVal){

  int barWidth = map(yVal, 0, 1023, 0, tftBoard.width());

  // clear previous rectangle everytime function is called
  tftBoard.fillRect(0, tftBoard.height()*.75, tftBoard.width(), 20, RA8875_BLACK);
  
  // update bar graph
  tftBoard.fillRect(0, tftBoard.height()*.75, barWidth, 20, RA8875_RED);

};

// pwm test helper function
void pwmTest(){
  // turn down pwm
  for (uint8_t i = 255; i != 0; i -= 5)
  {
    tftBoard.PWM1out(i);
    delay(10);
  }

  // turn up pwm
  for (uint8_t i = 0; i != 255; i += 5)
  {
    tftBoard.PWM1out(i);
    delay(10);
  }
};
