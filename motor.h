#ifndef MOTOR_H
#define MOTOR_H

#include "common.h"
#include "lcd.h"
#include "SPI.h"

#define CLOCKWISE 1 //Define clockwise and counter-clockwise
#define COUNTER_CLOCKWISE 0
#define DEG_PER_STEP 1.8 //Define step size
#define SPEED 5 //Define motor speed

//Set all half steps
#define STEP0 0b00111100
#define STEP1 0b00101110
#define STEP2 0b00101011
#define STEP3 0b00100111
#define STEP4 0b00110101
#define STEP5 0b00010111
#define STEP6 0b00011011
#define STEP7 0b00011110
signed char cstep = 0; //stores current step
signed int nrOfSteps = 0; //Stores total number of steps moved
int halfSteps[] = {STEP0, STEP1, STEP2, STEP3, STEP4, STEP5, STEP6, STEP7}; //Array with half steps
int wholeSteps[] = {STEP0, STEP2, STEP4, STEP6}; //Array with whole steps

void move(char direction);
void moveOld(int steps, char direction);
void moveDeg(double deg);
void moveMotorCont(void);
int stepToDegree(int steps);

#endif MOTOR_H
