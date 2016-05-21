#include "patternHandler.h"

 void stopAllPatterns() {
    // Reset all the pattern flags
    patternDone = 1;
    squarePatternDone = 1;
    straightPatternDone = 1;
    moveToWallPatternDone = 1;
    followWallPatternStart = 0;
    navigateMazePatternStart = 0;
    //Set default scan to scan only
    onlyScan = 1;
    //Reset the pattern stage 
    patternStage = 0;
    //stop the robot
    stop();
    //Reset scanner to origin
    resetScanner();
}

void updatePatterns() {
    
    if(navigateMazePatternStart){
        if(updateMap){
            degreesToTurn = moveSegment();
            lcdSetCursor(0x00);
            lcdWriteToDigitBCD(degreesToTurn, 3, 1);
            updateMap = 0;
            
        }
        if(navigateMazePattern(100, degreesToTurn)){
            updateMap = 1;
        }
    }
    //Check if the follow wall pattern flag has been set
    if (followWallPatternStart) {
        followWallPatternV2();
    }
    //Update the scanner and see if the sensor should be reset or not
    if (updateScanner()&&!onlyScan) {
        moveToWallPatternDone = 0;
        resetSensorToWallFollowFlag = 1;
        patternDone = 0;

    }
    //Check if the sensor should reset
    //If square pattern is not done update it
    if (!patternDone&&!squarePatternDone) {
        //Square pattern function returns an a 0 if its not done and a 1 if it is done

        squarePatternDone = moveSquarePattern();
        patternDone = squarePatternDone;
    }
    if (!patternDone&&!straightPatternDone) {
        //Square pattern function returns an a 0 if its not done and a 1 if it is done
        straightPatternDone = moveStraightPattern();
        patternDone = straightPatternDone;
    }
    if (!patternDone&&!moveToWallPatternDone) {
        //Square pattern function returns an a 0 if its not done and a 1 if it is done
        moveToWallPatternDone = moveTowardsWallPattern(lastReadSmallestStepDegree, lastReadSmallestDistance - 25);
        patternDone = moveToWallPatternDone;
        if (moveToWallPatternDone) {
            followWallPatternStart = 1;
        }
    }
    
    
    //Check if it should reset the sensor
    resetSensorToWallFollow();
}