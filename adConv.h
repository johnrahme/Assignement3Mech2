
#ifndef ADCONV_H
#define ADCONV_H
#include "common.h"
#include <math.h>
#include "lcd.h"
#include "timer0.h"
volatile bit conversionDone = 0; // Bit if conversion is done
void initializeADC(); //Ienit ADC
int readADCData();      //Read raw adc data
int readADCMeter();     // read adc data in meters
void startADCConversion(); // sets go bit and starts conversion
void printADCData(); 
int latestReadMeterValue = 0;


#endif ADCONV_H