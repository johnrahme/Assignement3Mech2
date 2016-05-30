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
    
    //Stop robot if it gets home
    
    if(updateSensorsFlag){
            //Update the LCD with the distance travelled and check bumper sensors
        updateSensors();// LOOK! Freezes program if not connected to robot
        updateSensorsFlag = 0;
    }
    
    //Special case for cliff check
    /*if((currentX == 3 && currentY == 3 && orientation == SOUTH)||(currentX == 2 && currentY == 2 && orientation == WEST)){
        followWallPatternStart = 0;
    }
    else{
        followWallPatternStart = 1;
    }*/
    if(currentX == 2&&currentY == 3 && orientation==WEST&&!updatingScannerPosition){
        if(getBumpDropSensor()){
           driveBack();
        __delay_ms(11*180);
        updateMap = 1;
        writeMapSegment(currentX, currentY, 0b00001100);
        currentX = prevX;
        currentY = prevY;
        //Hardcoded for bump at 2,4
         
         patternStage = 0; 
        }
        
        
        
    }
    //Should be virtual wall not lcdIRData
    if(updateVirtualWall){
        updateVirtualWall = 0;
        if(getVirtualWall()){
            virtualWallFound = 1;
        }
    }
        if(virtualWallFound&&!updatingScannerPosition){
            virtualWallFound = 0;
            LED0 = !LED0;
             driveBack();
            __delay_ms(2500); //Drive back further
            turnCCW();
            __delay_ms(11*180);
            updateMap = 1;
            char checkCurrentWall = 0;
            char checkPrevWall = 0;
            if(orientation == NORTH){
                checkCurrentWall = 0b00000010;
                checkPrevWall = 0b00001000;
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
                orientation = NORTH;
            }
            else if(orientation == WEST){
                checkCurrentWall = 0b00000100;
                checkPrevWall = 0b00000001;
                orientation = EAST;
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
        hasWallPrev = 0;
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
            turnBoost = 0;
            
            justLeftCliff = 1;
            
        }
        
    }
    
    if(navigateMazePatternStart){
        if(patternStage == 4){
            if(checkFrontWall()&&!updatingScannerPosition){
                if(currentX == 1 && currentY == 2 && orientation==EAST){
                    navigateMazePatternStart = 0;
                    followWallPatternStart = 0;
                    playSong4();
                    stop();
                    LED0 = !LED0;
                    updateMap = 0;
                }
                else{
                    updateMap = 1;
                    patternStage = 0;
                }
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
                hasWallPrev = 0;
            }else{
                hasWallPrev = 1;
            }
            if(getWallFollowDirection(0)==2){
                hasWallCurrent = 0;
            }else{
                hasWallCurrent = 1;
            }
            prevWallFollowDirection == wallFollowDirection;
            //****MIGHT NOT NEED THIS END****
            //int scannerSteps = getScannerLocation();
            updateMap = 0;
            
        }
        if(navigateMazePattern(100, degreesToTurn)){
            
            updateMap = 1;
            if(currentX == 1 && currentY == 2 && orientation==EAST){
                navigateMazePatternStart = 0;
                followWallPatternStart = 0;
                playSong4();
                stop();
                LED0 = !LED0;
                updateMap = 0;
            }
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
    
    if(followWallPatternStart&&((patternStage==2&&hasWallPrev&&!justLeftCliff)||(hasWallCurrent&&patternStage==3))&&!updatingScannerPosition&&!movingToCliff){
       followWallPatternV3(wallFollowDirection);
    }
    
    //Move to specific location
    moveToPositionFromOrigin(moveScannerTo);
    //updateScanner();
   
}