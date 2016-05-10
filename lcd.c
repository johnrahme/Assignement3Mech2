#include <htc.h>
#include "lcd.h"

//functions
//write controls to LCD
void lcdWriteControl(unsigned char databyte){
    EN = 0;
    RW = 0;
    RS = 0;
    LCD_DATA = databyte;
    EN = 1;
    EN = 0;
    __delay_ms(2);
}

//write data to LCD
void lcdWriteData(unsigned char databyte){
    EN = 0;
    RW = 0;
    RS = 1;
    LCD_DATA = databyte;
    EN = 1;
    EN = 0;
    __delay_ms(1);
}

//move the LCD cursor to a particular location
void lcdSetCursor(unsigned char address){
    address |= 0b10000000;		//format address command using mask
    lcdWriteControl(address);	//write address command
}

void lcdWriteString(char * s){
    // write characters
    while(*s) lcdWriteData(*s++);
}

//function accepts int between 0 and 9999 and writes it to lcd display in specified digits
    void lcdWriteToDigitBCD(signed int data, char digits, char showSign){
        unsigned int OnesDigit;
        unsigned char TensDigit, HundredDigit, ThousandDigit;
        signed char sign = 1;
        //Check the sign
        if(data<0)
            sign = -1;
        //load number to be converted into OnesDigit and clear TensDigit
        OnesDigit = data*sign;
        ThousandDigit = 0;
        HundredDigit = 0;
        TensDigit = 0;

        //Perform a BCD Conversion

        while(OnesDigit >= 1000){
                OnesDigit = OnesDigit - 1000;
                ThousandDigit++;
        }
        while(OnesDigit >= 100){
                OnesDigit = OnesDigit - 100;
                HundredDigit++;
        }
        while (OnesDigit >= 10){
                OnesDigit = OnesDigit - 10;
                TensDigit++;
        }
        
        //Check if plus or minus should be written to LCD
        if(sign == -1 && showSign)
            lcdWriteData((unsigned char)45);
        else if (sign == 1 && showSign){
            lcdWriteData((unsigned char)43);
        }
        //Check how many digits should be written to LCD
        if(digits>3)
            lcdWriteData((unsigned char)ThousandDigit + 48);
        if(digits>2)
            lcdWriteData((unsigned char)HundredDigit + 48);
        if(digits>1)
            lcdWriteData((unsigned char)TensDigit + 48);
        
        lcdWriteData((unsigned char)OnesDigit + 48);
    }

    //function initalises the LCD module - check that ADCON1 setting doesn't conflict
    void setupLCD(void){
        //Set port A to analogue and E to digital 
        PCFG3 = 0;
        PCFG2 = 0;
        PCFG1 = 1;
        PCFG0 = 1;
        
        PORTD = 0;				//set all pins on portd low
        PORTE = 0;				//set all pins on porte low

        TRISD = 0b00000000;		//set all pins to output
        TRISE = 0b00000000;		//set all pins to output

        //LCD Initialisation
        lcdWriteControl(0b00000001); //clear display
        lcdWriteControl(0b00111000); //set up display
        lcdWriteControl(0b00001100); //turn display on
        lcdWriteControl(0b00000110); //move to first digit
        lcdWriteControl(0b00000010); //entry mode setup

        //Clear screen

        lcdWriteControl(0b00000001);

    }