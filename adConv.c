#include "adConv.h"

void initializeADC(){
    //Set output on PORT A
    TRISA = 0b11111111;
    //Right justified
    ADFM = 1;
    //FOSC 32
    ADCS2 = 0;
    
    //Port A analog and port E digital
    PCFG3 = 0;
    PCFG2 = 0;
    PCFG1 = 1;
    PCFG0 = 0;
    
    ADCON0 = 0b10000001;
    
    //Activate ADC Interrupts
    
    ADIF = 0;
    ADIE = 1;
    PEIE = 1;
    GIE = 1;
}
//Read data from the ADC
int readADCData(){
    
       int ADCResult = (ADRESH<<8) + ADRESL ;   //Merging the MSB and LSB
       return ADCResult;
    
}

//Convert the read data to meters
int readADCMeter(){
    int raw = readADCData();
    double rawInverted =  1.0/((double) raw);
    //For 5 .0
    double k = 12282;
    //For 3.3 volt
    //double k = 18609; 
    double m = -1.6803;
    double result = rawInverted*k + m;
    int resultInInt = (int) round(result);
    return resultInInt;
}
//Start the ADC coversion
void startADCConversion(){
       //acquisition time
       __delay_us(300);
       //Set GO/DONE
       GO = 1;
}

void printADCData(){ 
            int result = readADCData(); // Read data from ADC
            int resultInMeters = readADCMeter(); //Read data and convert to meters
            if(resultInMeters > 15 && resultInMeters){
                latestReadMeterValue = resultInMeters;
            }  
            if(updateLcdIRData){ // Check LCD refresh rate and print to LCD
                updateLcdIRData = 0;
                lcdSetCursor(0x00);
                //lcdWriteString("Raw:");
                //lcdWriteToDigitBCD(result,4,0);
                //lcdSetCursor(0x09);
                //lcdWriteString("=>");
                lcdWriteToDigitBCD(resultInMeters,3,0); // Write the adc data in meters to LCD
                lcdWriteString("cm");
                
            }
            // Restart the ADC conversion
            startADCConversion();    
}