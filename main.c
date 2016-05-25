/*********************************************************************************
	Description: Controls stepper motor from debounced buttons
	Uses: DSX Experimenter board and SMPM
	Connections:
				iRobot - DSX Kit Addon Module

	Author: Kyle Alvarez

**********************************************************************************/
#include "common.h"
#include "ser.h"
#include "iRobot.h"
#include "timer0.h"
#include "lcd.h"
#include "SPI.h"
#include "motor.h"
#include "adConv.h"
#include "patternHandler.h"
#include "music.h"
#include "eeprom.h"

#pragma config BOREN = OFF, CPD = OFF, WRT = OFF, FOSC = HS, WDTE = OFF, CP = OFF, LVP = OFF, PWRTE = OFF

#define TMR0_VAL 100	//timer0 start count
#define DEG_PER_SEC 85;


// Interrupt service routine
void interrupt isr(void)
{
    //Serial interrupt handler
    ser_int();

    //Timer 1
    if(TMR0IF){
        TMR0IF = 0;
        TMR0 = TMR0_VAL;
        //User timer0 source function to flash a led
        flashLed();
        //Increment counter for the current pattern
        updateMovePattern();
        //Debounce the buttons
        debounceButtons();
        //Increment SM counter
        stepperMotorCounter();
        //Increment timer for lcd refresh
        lcdRefresh();
        
        //moveMotorCont();
    }
    if(ADIF){
        conversionDone = 1; //Set conversion done flag
        ADIF = 0;
    }
}

void setup(void){
    __delay_ms(5000);   // Safety delay while iRobot serial buffers are streaming stuff
    setupSPI();
    ser_init(); 
      //Timer initialization
    initializeTimer0();
    setupIRobot();
    // Set RB0-RB3 as pushbuttons and RB4-RB5 as LED:S
    TRISB = 0b00001111;  //For LED:s and pushbuttons  
    
    
    setToMotorCW();
    setupLCD();
    initializeADC();
    setupSongs();
}

void main (void){
    setup();
    LED1 = 0;
    LED0 = 0;
    writeAdvancedMap();
    setStartPos(WEST,1,2);
    victimsFound = 0;
    int moveToVariable = 0;
    //moveOld(100,0);
    lcdSetCursor(0x08);
    lcdWriteToDigitBCD(6,4,0);
    
    // Start the adc conversion
    startADCConversion();
    while(1){   
        //move(10,0);
        //Check ADC coversion
        if(conversionDone){ //Check conversion done flags
            conversionDone = 0; 
            printADCData(); //Prints the conversion data to the LCD
        }

        
        if(pb1Pressed){
            // Start the straight pattern
            distanceTraveled = 0; //added in to 0 the total distance traveled at the start of the function
            navigateMazePatternStart = 1;
            followWallPatternStart = 1;
            followPatternStage = 0;
            patternDone = 0;
            pb1Pressed = 0;
            LED0 = !LED0;
            
        }  
        if(pb2Pressed){
            setScannerSpeed(1);
            
            if(!updatingScannerPosition){
                moveScannerTo = moveToVariable;
                updatingScannerPosition = 1;
            } 
            pb2Pressed = 0;
            
        }
        if(pb3Pressed){
            moveToVariable +=10;
            lcdSetCursor(0x45);
            lcdWriteToDigitBCD(moveToVariable,3,1);
            pb3Pressed = 0;
        }
        if(pb0Pressed){
            moveToVariable -=10;
            lcdSetCursor(0x45);
            lcdWriteToDigitBCD(moveToVariable,3,1);
            pb0Pressed = 0;
        }
        
        //Use patternHandler to update the patterns
        updatePatterns();
        
        //Update the LCD with the distance travelled and check bumper sensors
        if(!scanRunning&&!movingToWall){
            //Update the LCD with the distance travelled and check bumper sensors
            //updateSensors();// LOOK! Freezes program if not connected to robot
        }
        
    }
    

}
