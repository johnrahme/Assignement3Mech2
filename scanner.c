#include "scanner.h"

void setScannerSpeed(char speed) {
    SM_COUNTER_TIME = speed;
}

void checkClosestDistance() {
    if (latestReadMeterValue < smallestDistance) { //if smallest recorded distance is greater than distance just read by scanner;
        smallestDistance = latestReadMeterValue; //last read distance becomes the new mallest distance read
        smallestValueStep = scanStepNumber; //smallest step number changes accordingly to match distance just scanned
    }
}

void resetScanner() {
    //Reset all values of the scanner and move it back to the origin
    smallestDistance = 20000;
    scanStepNumber = 0;
    smallestValueStep = 0;
    scanRunning = 0;
    stepsToMove = 400;
    movingToWall = 0;
    lastReadSmallestDistance = 20000;
    lastReadSmallestStepDegree = 0;

    resetSensorToWallFollowFlag = 0;
    //Just to move the motor back to its starting position
    moveOld(stepsFromOrigin, 1);
    stepsFromOrigin = 0;
}

void updateScannerBuffer() {
    //Buffer the values of smallest distance and smallest step to use for later 
    scanStepNumber = 0; //reset scanner/stepper motor step number value
    lastReadSmallestDistance = smallestDistance;
    lastReadSmallestStepDegree = stepToDegree(smallestValueStep);
    smallestDistance = 20000;
    smallestValueStep = 0;
}

char updateScanner() {

    //First rotate and store closes wall
    if (RTC_MOVE_SM_FLAG && scanRunning) {
        RTC_MOVE_SM_FLAG = 0;
        if (scanStepNumber < stepsToMove) { //if scanner step number is less than steps to move
            scanStepNumber++; //increment scanner step number
            move(0); //move in direction 0(clockwise)
            checkClosestDistance();
        } else { //otherwise;
            scanStepNumber = 0; //clear scanner step number
            scanRunning = 0; //clear scan running
            movingToWall = 1; //robot rotates to face wall
            stepsFromOrigin = 0;
        }

    }
    //Then rotate back
    if (movingToWall && RTC_MOVE_SM_FLAG) { //if scanner is rotating to face wall and stepper motor flag is raised
        RTC_MOVE_SM_FLAG = 0; //clear flag
        scanStepNumber++; //increment scanner/stepper motor step number
        if (scanStepNumber < (stepsToMove - smallestValueStep)) { //if scanner/stepper motor step number is less than the difference between steps required to move and the smallest stepper value
            move(1); //move in direction 1(counter-clockwise))
        } else { //otherwise;
            movingToWall = 0; //do not rotate scanner to face wall
            stepsFromOrigin = smallestValueStep;
            updateScannerBuffer();
            return 1; //return 1(meaning sequence is complete)
        }
    }

    return 0; //return 0(meaning sequence is incomplete)

}

void resetSensorToWallFollow() {
    if (RTC_MOVE_SM_FLAG && resetSensorToWallFollowFlag) {
        RTC_MOVE_SM_FLAG = 0;
        stepsToMove = WALL_FOLLOW_ANGLE - stepsFromOrigin;
        char direction = 0;
        if (stepsToMove < 0) {
            stepsToMove *= -1;
            direction = 1;
        }
        if (scanStepNumber < stepsToMove) { //if scanner step number is less than steps to move
            scanStepNumber++; //increment scanner step number
            move(direction); //move in direction 0(clockwise)
        } else { //otherwise;
            stepsFromOrigin = WALL_FOLLOW_ANGLE;
            scanStepNumber = 0;
            resetSensorToWallFollowFlag = 0;
        }

    }
}
