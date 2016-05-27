#ifndef MOTOR_H
#define MOTOR_H

#include "common.h"
#include "lcd.h"
#include "SPI.h"

#define CLOCKWISE 1 //Define clockwise and counter-clockwise
#define COUNTER_CLOCKWISE 0
#define DEG_PER_STEP 1.8 //Define step size
#define SPEED 5 //Define motor speed


void move(char direction);
void moveDeg(double deg);
int stepToDegree(int steps);

#endif MOTOR_H
