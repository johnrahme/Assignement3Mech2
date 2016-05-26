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
    
    if(1){
            //Update the LCD with the distance travelled and check bumper sensors
        updateSensors();// LOOK! Freezes program if not connected to robot
    }
    
    if(navigateMazePatternStart){
        if(patternStage == 3){
            if(checkFrontWall()){
                updateMap = 1;
                patternStage = 0;
            }
        }
        if(updateMap){
            degreesToTurn = moveSegment();
            if(currentX == 1 && currentY == 4){
                wallFollowDirection = 0;
                updatingScannerPosition = 1;
                moveScannerTo = -50;
            }
            //int scannerSteps = getScannerLocation();
            updateMap = 0;
            
        }
        if(navigateMazePattern(100, degreesToTurn)){
            updateMap = 1;
        }
        if(enteredFrontStage){
            enteredFrontStage = 0;
            updatingScannerPosition = 1;
            moveScannerTo = 0;
        }
        else if(enteredFollowStage){
            enteredFollowStage = 0;
            updatingScannerPosition = 1;
            if(wallFollowDirection==0){
                moveScannerTo = -30;
            }
            else{
                moveScannerTo = 30;
            }
        }
    }
    //Follow wall if robot is moving straight and not turning the scanner
    
    if(followWallPatternStart&&patternStage==2&&!updatingScannerPosition){
       followWallPatternV3(wallFollowDirection);
    }
    
    //Move to specific location
    moveToPositionFromOrigin(moveScannerTo);
    //updateScanner();
   
}