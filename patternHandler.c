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
    
    
    if(updateSensorsFlag){
            //Update the victim sensors
        updateSensors();// LOOK! Freezes program if not connected to robot
        updateSensorsFlag = 0;
    }
    
    // Check for bump sensor in segment 2,3
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
    //Check the flag for updating virtual wall sensor
    if(updateVirtualWall){
        updateVirtualWall = 0;
        if(getVirtualWall()){
            // If virtual wall detected set flag
            virtualWallFound = 1;
        }
    }
        // If the scanner position is not updating and wall is found move back and turn around
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
            // Write wall to eeprom so that it does not need to check the virtual wall next time
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

            //Write the new wall to eeprom
            writeMapSegment(currentX, currentY, (checkCurrentWall|currWalls));
            writeMapSegment(prevX, prevY, (checkPrevWall|prevWalls));
            // Reset current position to previous position
            currentX = prevX;
            currentY = prevY;
            patternStage = 0;

        }
    // Dont look for wall when heading towards 2,2 when moving WEST
    if(currentX == 2 && currentY == 2 && orientation == WEST){
        hasWallPrev = 0;
    }

    // Hard coded part for cliff, when in 2,4 moving EAST
    if(currentX == 2 && currentY == 4 && orientation == EAST&&movingStraight){
        movingToCliff = 1;
        // Reset the distance reader
        if(resetDistanceReader){
            resetDistanceReader = 0;
            getTraveledDistance();
            patternStage = 0;
            distanceToCliff = 0;
        }
        navigateMazePatternStart = 0;
        //Update the find cliff pattern until it returns a 1
        if(findCliffPattern()){
            
            navigateMazePatternStart = 1;
            updateMap = 1;
            patternStage = 0;
            // set the cliff to a wall
            writeMapSegment(2,4,0b00001011);
            //Make 1,3, south a wall, Small fix so robot would not go back and check that area again
            writeMapSegment(1,3,0b00001111);
            currentX = prevX;
            currentY = prevY;
            movingToCliff = 0;
            // Old variable set to make the robot turn a little extra after cliff
            turnBoost = 0;
            
            justLeftCliff = 1;
            
        }
        
    }
    
    //Check the navigate maze flag
    if(navigateMazePatternStart){
        // If in fron check stage
        if(patternStage == 4){
            // Check front for wall
            if(checkFrontWall()&&!updatingScannerPosition){
                // If robot is home stop and play song
                if(currentX == 1 && currentY == 2 && orientation==EAST){
                    navigateMazePatternStart = 0;
                    followWallPatternStart = 0;
                    playSong4();
                    stop();
                    LED0 = !LED0;
                    updateMap = 0;
                }
                // Otherwise update map and restart cycle
                else{
                    updateMap = 1;
                    patternStage = 0;
                }
            }
        }
        // If the map is to be updated, that is start heading to a new segment
        if(updateMap){
            // Use map navigation to find out how many degrees to turn 
            degreesToTurn = moveSegment();

            // Find out what direction to wall follow
            if(getWallFollowDirection(0)==1){
                wallFollowDirection = 1;
            }
            else if(getWallFollowDirection(0)==0){
                wallFollowDirection = 0;
                
            }
            else{
                wallFollowDirection = 2;
            }

            // Check if previous sgement had a wall
            if(getWallFollowDirection(1)==2){
                hasWallPrev = 0;
            }else{
                hasWallPrev = 1;
            }
            // Check if current sgement had a wall
            if(getWallFollowDirection(0)==2){
                hasWallCurrent = 0;
            }else{
                hasWallCurrent = 1;
            }
            prevWallFollowDirection == wallFollowDirection;
            updateMap = 0;
            
        }
        // Update the navigate maze pattern with the degrees to turn 
        if(navigateMazePattern(100, degreesToTurn)){
            // When the pattern is done update the map for next segment
            updateMap = 1;
            // If home play sound and stop
            if(currentX == 1 && currentY == 2 && orientation==EAST){
                navigateMazePatternStart = 0;
                followWallPatternStart = 0;
                playSong4();
                stop();
                LED0 = !LED0;
                updateMap = 0;
            }
        }
        //Update the ir position when front check stage is entered
        if(enteredFrontStage){
            enteredFrontStage = 0;
            updatingScannerPosition = 1;
            moveScannerTo = 0;
        }
        // Update the ir position when wall follow stage is entered
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
    
    // Follow wall if there is a wall to follow and if robot is moving straight and not turning the scanner
    if(followWallPatternStart&&((patternStage==2&&hasWallPrev&&!justLeftCliff)||(hasWallCurrent&&patternStage==3))&&!updatingScannerPosition&&!movingToCliff){
       followWallPatternV3(wallFollowDirection);
    }
    
    //Move scanner to specific location
    moveToPositionFromOrigin(moveScannerTo);
   
}