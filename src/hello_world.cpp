/*
tft test
*/

#include <SPI.h>
#include "Adafruit_RA8875.h"
#include "Adafruit_GFX.h"

// define interrupt 
// interrupt allows the progrom to continue running/looping while waiting for events
// as in the case of waiting for touch events on the tft
#define interrupt 3

//define chip select
// SPI uses CS pin to have isolated and unique devices 
#define chipSelect 10

// reset pin brings the VIN down to LOW effectively shutting ogg power and resetting device
#define reset 9

// digital 3, 9, 10 are used for the auxilliary pins
// the remaining pins used will be on ICSP 1, which is the group of 6 pins near back of UNO or in middle of larger board like DUE
//ICSP in-curcuit Serial Programming

//initialize the adafruit driver board library
//first arg is the chip select
//second arg is interrupt 
//uint8_t means 8 bit number 0-255
Adafruit_RA8875 tftBoard = Adafruit_RA8875(chipSelect, interrupt);

//declare 16 bit variables to represent screen coords, need coords > 255 because pixel size is 800X480
uint16_t tx, ty;

void setup(){
    
    Serial.begin(9600);
    Serial.println("Start serial");

    //initialze using .begin method or class variable??
    if (!tftBoard.begin(RA8875_800x480)){
        // begin method of the Adafruit_RA8875 class is responbsible for verifying appropriate screen size 
        // is selected, setting the pinMode of the CS and Int, setting the serial clack speed, check for serial 
        // transaction, pinging register (0)
        Serial.println("Failed to Initialize driver board");
        // keep looping until this initializes
        while(1);
}
    //else
    Serial.println("TFT Initialized");
    
    //display on will write to the regsiter 0x01 and the BITWISE OR will take 0x00 RA8875_PWRR_NORMAL and 0x80 RA8875_PWRR_DISPON
    // the result is 0x80
    //    0000 0000
    //    1000 0000
    //-------------
    //    1000 0000
    // 
    //data sheet shows some information about PWRR registers
    // 
    tftBoard.displayOn(true);
    //writeReg(0x01, 0x00 | 0x80);
    // i want to be able to print the register PWRR
    uint8_t pwrr_reg = tftBoard.readReg(RA8875_PWRR);
    Serial.println(pwrr_reg);
}

void loop(){};
