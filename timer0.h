#ifndef TIMER0_H
#define TIMER0_H
#include "common.h"

#define DEBOUNCE_REQ_COUNT 10 // Debounce for 10 ms
#define LCD_REFRESH_RATE 100 // set refresh rate 
#define VIRTUAL_WALL_REFRESH_RATE 50

volatile unsigned int rtcCounter = 0;

volatile bit pb0Pressed = 0; // Bit flag to signal if the pushbutton is pressed
volatile bit pb0Released = 0; // Bit to signal if the pushbutton has been released
volatile unsigned char pbCounter = 0; //Button counter

volatile bit pb1Pressed = 0; 
volatile bit pb1Released = 0; 
volatile unsigned char pbCounter2 = 0; 

volatile bit pb2Pressed = 0; 
volatile bit pb2Released = 0; 
volatile unsigned char pbCounter3 = 0;

volatile bit pb3Pressed = 0;
volatile bit pb3Released = 0; 
volatile unsigned char pbCounter4 = 0;

volatile bit updateVirtualWall = 0;
volatile unsigned char virtualWallCounter = 0;

volatile bit updateLcdIRData = 0; //Update refresh bit flag
volatile bit updateSensorsFlag = 0; //Sensors update at the same time as LCD
volatile bit updateBumpSensorFlag = 0;
volatile unsigned char irLcdCounter = 0; //LCD counters

volatile bit RTC_FLAG_MOVE_PATTERN = 0;
volatile unsigned int RTC_MOVE_PATTERN_COUNTER = 0;
volatile unsigned int MOVE_PATTERN_TIME = 1;

volatile bit RTC_FLAG_FOLLOW_PATTERN = 0;
volatile unsigned int RTC_FOLLOW_PATTERN_COUNTER = 0;
volatile unsigned int FOLLOW_PATTERN_TIME = 1;

volatile bit RTC_MOVE_SM_FLAG = 0;
volatile unsigned char RTC_MOVE_SM_COUNTER = 0;
volatile char SM_COUNTER_TIME = 5;
       
volatile bit RTC_LOST_WALL_FLAG = 0;
volatile unsigned int RTC_LOST_WALL_COUNTER = 0;
volatile unsigned int LOST_WALL_TIME = 0;


void initializeTimer0();
void debounceButtons();
void lcdRefresh();
void flashLed();
void updateMovePattern();
void stepperMotorCounter();


  #endif TIMER0_H