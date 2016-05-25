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
            updateMap = 0;
            
        }
        if(navigateMazePattern(100, degreesToTurn)){
            updateMap = 1;
        }
    }
    //Update the scanner and see if the sensor should be reset or not
    
    if(followWallPatternStart&&movingStraight){
       followWallPatternV3();
    }
    
    //Move to specific location
    moveToPositionFromOrigin(moveScannerTo);
    //updateScanner();
   
}