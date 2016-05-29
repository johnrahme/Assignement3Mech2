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
    if(currentX == 2&&currentY == 3 && orientation==WEST&&getBumpDropSensor()){
        driveBack();
        __delay_ms(2080);
        updateMap = 1;
        writeMapSegment(currentX, currentY, 0b00001100);
        currentX = prevX;
        currentY = prevY;
        //Hardcoded for bump at 2,4
         
         patternStage = 0;
        
        
    }
    if(getVirtualWall()){
         driveBack();
        __delay_ms(2080);
        turnCCW();
        __delay_ms(12*180);
        updateMap = 1;
        char checkCurrentWall = 0;
        char checkPrevWall = 0;
        if(orientation == NORTH){
            checkCurrentWall = 0b00000010;
            checkPrevWall = 0b00000001;
            orientation = SOUTH;
        }
        else if(orientation == EAST){
            checkCurrentWall = 0b00000001;
            checkPrevWall = 0b00000100;
            orientation = WEST;
        }
        else if(orientation == SOUTH){
            checkCurrentWall = 0b00001000;
            checkPrevWall = 0b00000010;
            orientation == NORTH;
        }
        else if(orientation == WEST){
            checkCurrentWall = 0b00000100;
            checkPrevWall = 0b00000001;
            orientation == EAST;
        }
        char currWalls = readMapSegment(currentX, currentY);
        char prevWalls = readMapSegment(prevX, prevY);
        
        writeMapSegment(currentX, currentY, (checkCurrentWall|currWalls));
        writeMapSegment(prevX, prevY, (checkPrevWall|prevWalls));
        
        currentX = prevX;
        currentY = prevY;
        patternStage = 0;
        
    }
    // Dont look for wall when heading towards 2,2 when moving WEST
    if(currentX == 2 && currentY == 2 && orientation == WEST){
        hasWall = 0;
    }
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
            //writeMapSegment(2,3,0b00000100);
            //Make 1,3, south a wall
            writeMapSegment(1,3,0b00001111);
            currentX = prevX;
            currentY = prevY;
            movingToCliff = 0;
            turnBoost = 20;
            
            justLeftCliff = 1;
            
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
            //*****MIGHT NOT NEED THIS START***
            if(getWallFollowDirection(1)==2){
                hasWall = 0;
            }else{
                hasWall = 1;
            }
            prevWallFollowDirection == wallFollowDirection;
            //****MIGHT NOT NEED THIS END****
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
    
    if(followWallPatternStart&&((patternStage==2&&hasWall&&!justLeftCliff)||patternStage==3)&&!updatingScannerPosition&&!movingToCliff){
       followWallPatternV3(wallFollowDirection);
    }
    
    //Move to specific location
    moveToPositionFromOrigin(moveScannerTo);
    //updateScanner();
   
}