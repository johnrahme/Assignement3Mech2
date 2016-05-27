#include "patternHandler.h"

 void stopAllPatterns() {
    // Reset all the pattern flags
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
    
    //Special case for cliff check
    /*if((currentX == 3 && currentY == 3 && orientation == SOUTH)||(currentX == 2 && currentY == 2 && orientation == WEST)){
        followWallPatternStart = 0;
    }
    else{
        followWallPatternStart = 1;
    }*/
    if(currentX == 2 && currentY == 4 && orientation == EAST&&movingStraight){
        movingToCliff = 1;
        if(resetDistanceReader){
            resetDistanceReader = 0;
            getTraveledDistance();
            patternStage = 0;
            distanceToCliff = 0;
        }
        navigateMazePatternStart = 0;
        if(findCliffPattern()){
            
            navigateMazePatternStart = 1;
            updateMap = 1;
            patternStage = 0;
            // set the cliff to a wall
            writeMapSegment(2,4,0b00001011);
            writeMapSegment(2,3,0b00000100);
            //Make 1,3, south a wall
            writeMapSegment(1,3,0b00000110);
            currentX = prevX;
            currentY = prevY;
            movingToCliff = 0;
        }
        
    }
    
    if(navigateMazePatternStart){
        if(patternStage == 4){
            if(checkFrontWall()){
                updateMap = 1;
                patternStage = 0;
            }
        }
        if(updateMap){
            degreesToTurn = moveSegment();
            if(getWallFollowDirection(0)==1){
                wallFollowDirection = 1;
            }
            else if(getWallFollowDirection(0)==0){
                wallFollowDirection = 0;
                
            }
            else{
                wallFollowDirection = 2;
            }
            if(getWallFollowDirection(1)==2){
                hasWall = 0;
                lcdSetCursor(0x08);
                lcdWriteString("No wall");
            }else{
                hasWall = 1;
                lcdSetCursor(0x08);
                lcdWriteString("Has wall");
            }
            prevWallFollowDirection == wallFollowDirection;
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
    
    if(followWallPatternStart&&((patternStage==2&&hasWall)||patternStage==3)&&!updatingScannerPosition&&!movingToCliff){
       followWallPatternV3(wallFollowDirection);
    }
    
    //Move to specific location
    moveToPositionFromOrigin(moveScannerTo);
    //updateScanner();
   
}