
#ifndef SCANNER_H
#define	SCANNER_H

#include "common.h"
#include "motor.h"
#include "adConv.h"
#include "timer0.h"

#define WALL_FOLLOW_ANGLE 55
int smallestDistance = 20000;
int scanStepNumber = 0;
int smallestValueStep = 0;
int scanRunning = 0;
int stepsToMove = 400;
int movingToWall = 0;
int stepsFromOrigin = 0;
int lastReadSmallestDistance = 20000;
int lastReadSmallestStepDegree = 0;

char updatingPosition = 0;



char resetSensorToWallFollowFlag = 0;
int moveScannerTo = 0;

char updateScanner();
void updateScannerOldWay();
void setScannerSpeed(char speed);
void resetSensorToWallFollow();
void moveToPositionFromOrigin(int steps);

void resetScanner();


#endif	/* SCANNER_H */

