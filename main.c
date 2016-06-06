
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
    spi_transfer(0);
}

void main (void){
    setup();
    LED1 = 0;
    LED0 = 0;
    //Setup all songs
    setupSongs();
    //Write map to eeprom
    writeAdvancedMap();
    //Set the start position and orientation
    char startOrientation = WEST;
    char startX = 1;
    char startY = 2;
    setStartPos(startOrientation,startX,startY);
    // Print current position to lcd
    printPosition(startX, startY, startOrientation); 
    setScannerSpeed(3);
    victimsFound = 0;
    int moveToVariable = 0;
    
    
    
    
    
    // Start the adc conversion
    startADCConversion();
    while(1){
       
        //move(10,0);
        //Check ADC coversion
        if(conversionDone){ //Check conversion done flags
            conversionDone = 0; 
            printADCData(); //Prints the conversion data to the LCD
        }

        
        if(pb0Pressed){
            //Start the navigate maze pattern
            robotSpeed = FAST;
            playSong3(); // Star song!
            setStartPos(startOrientation,startX,startY);
            distanceTraveled = 0; //added in to 0 the total distance traveled at the start of the function
            navigateMazePatternStart = 1;
            followWallPatternStart = 1;
            followPatternStage = 0;
            pb0Pressed = 0;
            
            
        }  

        //Pb1-pb3 Sets the start position in the maze. This is for debugging if we want to start somewhere else.
        if(pb1Pressed){
            if(startX==4){
                startX = 1;
            }
            else{
                startX ++;
            }
            printPosition(startX, startY, startOrientation);

            pb1Pressed = 0;
            
            
            
        }
        if(pb2Pressed){
            if(startY==5){
                startY = 1;
            }
            else{
                startY ++;
            }
            printPosition(startX, startY, startOrientation);
            
            pb2Pressed = 0;
        }
        if(pb3Pressed){
            turnCCW();
            __delay_ms(11*180);
           stop();
            if(startOrientation==3){
                startOrientation = 0;
            }
            else{
                startOrientation++;
            }
            printPosition(startX, startY, startOrientation);
            
            pb3Pressed = 0;
        }
        
        //Use patternHandler to update the patterns
        updatePatterns();
        
       
        
    }
    

}
