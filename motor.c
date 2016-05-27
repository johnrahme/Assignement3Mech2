/*
 * File:   motor.c
 * Author: JohnMac
 *
 * Created on den 1 april 2016, 16:36
 */

#include "motor.h"

void initializeMotor(){
    //Initialize motor to step 0 and write to LCD
}

void move(char direction){
    if(direction == CLOCKWISE && currentDirection != CLOCKWISE){
        // USes she SPI to set the motor to CW
        SELECT_SM();
        setToMotorCW();
        
    }
    else if(direction == COUNTER_CLOCKWISE && currentDirection !=COUNTER_CLOCKWISE){
        // USes she SPI to set the motor to CCW
        SELECT_SM();
        setToMotorCCW();
    }
    
    SELECT_SM();
    SM_STEP();
    SELECT_NONE();
}


//Function to convert steps to degrees
int stepToDegree(int steps){
    int degrees = (int) steps*(DEG_PER_STEP/2.0); 
    return degrees;
}