#include "iRobot.h"


void setupIRobot(void){
    ser_putch(START); //start create robot
    __delay_ms(5);
    ser_putch(FULL); // Set to full mode
    __delay_ms(5);
}

//Drive straight forward
void drive(void){
    ser_putch(DRIVE); __delay_ms(5); ser_putch(0); __delay_ms(5); ser_putch(200); __delay_ms(5); ser_putch(127); __delay_ms(5); ser_putch(255);__delay_ms(5);
}
void driveBack(void){
    ser_putch(DRIVE); __delay_ms(5); ser_putch(255); __delay_ms(5); ser_putch(56); __delay_ms(5); ser_putch(127); __delay_ms(5); ser_putch(255);__delay_ms(5);
}
void driveSlow(){
    ser_putch(DRIVE); __delay_ms(5); ser_putch(0); __delay_ms(5); ser_putch(100); __delay_ms(5); ser_putch(127); __delay_ms(5); ser_putch(255);__delay_ms(5);
}
//This function sets the wheel speeds independently
void turnAndDriveDirect(int rightVelocity, int leftVelocity){
    //High and low byte for right wheel
    char highByteR = 0;
    char lowByteR = 0;
    //High and low byte for left wheel
    char highByteL = 0;
    char lowByteL = 0;
    //Sets speed of Right Wheel
    if(rightVelocity <0){
        highByteR = -rightVelocity/256;
        lowByteR = -rightVelocity%256;
        highByteR = highByteR ^ 0b11111111;
        lowByteR = lowByteR ^ 0b11111111;
    }
    else{
        highByteR = rightVelocity/256;
        lowByteR = rightVelocity%256;
    }
    //Sets speed of left Wheel
    if(rightVelocity <0){
        highByteL = -leftVelocity/256;
        lowByteL = -leftVelocity%256;
        highByteL = highByteL ^ 0b11111111;
        lowByteL = lowByteL ^ 0b11111111;
    }
    else{
        highByteL = leftVelocity/256;
        lowByteL = leftVelocity%256;
    }
    // Send the ser putch values
    ser_putch(DRIVE_DIRECT); __delay_ms(5); ser_putch(highByteR); __delay_ms(5); ser_putch(lowByteR); __delay_ms(5); ser_putch(highByteL); __delay_ms(5); ser_putch(lowByteL);__delay_ms(5);
}

//Makes the robot turn and drive with a specific radius
void turnAndDrive(int radius){
    //Radius high byte and low byte
    char highByte = 0;
    char lowByte = 0;
    if(radius <0){
        highByte = -radius/256;
        lowByte = -radius%256;
        highByte = highByte ^ 0b11111111;
        lowByte = lowByte ^ 0b11111111;
    }
    else{
        highByte = radius/256;
        lowByte = radius%256;
    }
    ser_putch(DRIVE); __delay_ms(5); ser_putch(0); __delay_ms(5); ser_putch(200); __delay_ms(5); ser_putch(highByte); __delay_ms(5); ser_putch(lowByte);__delay_ms(5);
}
//Turn clockwise 
void turnCW(){
    ser_putch(DRIVE); __delay_ms(5); ser_putch(0); __delay_ms(5); ser_putch(200); __delay_ms(5); ser_putch(255); __delay_ms(5); ser_putch(255);__delay_ms(5);
}

//Turn counter clockwise
void turnCCW(){
    ser_putch(DRIVE); __delay_ms(5); ser_putch(0); __delay_ms(5); ser_putch(200); __delay_ms(5); ser_putch(0); __delay_ms(5); ser_putch(1);__delay_ms(5);
}
//Stop the robot
void stop(void){
    ser_putch(DRIVE); __delay_ms(5); ser_putch(0); __delay_ms(5); ser_putch(0); __delay_ms(5); ser_putch(0); __delay_ms(5); ser_putch(0);__delay_ms(5);
}

//Move forward a specified number of centimeters
void moveDistanceForward(int centimeters){
    RTC_MOVE_PATTERN_COUNTER = 0; //Reset the counter
    // 21053/4/100 = 52.6325 ---> milliseconds to move one centimeter
    
    float timeToMoveOneCentimeter = 52.6325; // Should probably be a float number instead
    int totalTimeToMove = centimeters*timeToMoveOneCentimeter;
    //Set the time for the counter to wait until next step in pattern
    MOVE_PATTERN_TIME = totalTimeToMove;
    drive();
}
//Move forward a specified number of centimeters
void moveDistanceBackwards(int centimeters){
    RTC_MOVE_PATTERN_COUNTER = 0; //Reset the counter
    // 21053/4/100 = 52.6325 ---> milliseconds to move one centimeter
    float timeToMoveOneCentimeter = 52.6325; // Should probably be a float number instead
    int totalTimeToMove = centimeters*timeToMoveOneCentimeter;
    //Set the time for the counter to wait until next step in pattern
    MOVE_PATTERN_TIME = totalTimeToMove;
    driveBack();
}
//Turn degrees clockwise a specified amount of degrees
void turnDegreesCW(int degrees){
    RTC_MOVE_PATTERN_COUNTER = 0; //Reset the counter
    // 1130/90 = 12.55 ---> milliseconds to move one degree
    int timeToMoveOneDegree = 11; // Should probably be a float number instead
    int totalTimeToTurn = degrees*timeToMoveOneDegree;
    //Set the time for the counter to wait until next step in pattern
    MOVE_PATTERN_TIME = totalTimeToTurn;
    turnCW();
    
}
//Turn degrees counter clockwise a specified amount of degrees
void turnDegreesCCW(int degrees){
    RTC_MOVE_PATTERN_COUNTER = 0; //Reset the counter
    // 1130/90 = 12.55 ---> milliseconds to move one degree
    float timeToMoveOneDegree = 12; // Should probably be a float number instead
    int totalTimeToTurn = degrees*timeToMoveOneDegree;
    //Set the time for the counter to wait until next step in pattern
    MOVE_PATTERN_TIME = totalTimeToTurn;
    turnCCW();
    
}
//-----MOVE PATTERNS START-----
//Follow wall pattern for the maze
char followWallPatternV3(char right){
    //Check if the move pattern flag has been raised
    if(followPatternStage == 0|| (followPatternStage == 1 && RTC_FLAG_FOLLOW_PATTERN)){
        //Reset counter
        RTC_FOLLOW_PATTERN_COUNTER = 0; 
        FOLLOW_PATTERN_TIME = 10; //How often to update
        int valueOff = latestReadMeterValue-65;
        valueOff*10; // Convert To millimeters
        int speedRightWheel = 0;
        int speedLeftWheel = 0;
        char divideBy = 1;
        char times = 3;
        char wallToFar = 0;
        
        //If the distance is far make the divideBy factor bigger so as not to turn to fast
        if((valueOff>50)){
            wallToFar = 1;
        }
        //If the robot is really close to the edge turn really fast
        
        
        //Set the wheel speed
        if(wallToFar){
            speedRightWheel = 200;
            speedLeftWheel = 200;
            
        }
        else{
            speedRightWheel = 200+valueOff/divideBy*times;
            speedLeftWheel = 200-valueOff/divideBy*times;
        }
        
         //Check if it lost the wall
         
        
        lastValueOff = valueOff;
        //Use turn and drive direct functions
        if(right){
             turnAndDriveDirect(speedLeftWheel,speedRightWheel);
        }
        else{
             turnAndDriveDirect(speedRightWheel,speedLeftWheel);
        }
       
        //Do not increment patternStage since we want this function to run forever, unless its stage 0
        if(followPatternStage == 0){
             followPatternStage++;
        }
       
        //Reset Pattern Flag
        RTC_FLAG_FOLLOW_PATTERN = 0;
    }
}

char findCliffPattern(){
    
    if(patternStage == 0){
        distanceToCliff +=getTraveledDistance();
        driveSlow();
        
        if(getCliffSensors()){
            
            patternStage++;
        }
        lcdSetCursor(0x0A);
        lcdWriteToDigitBCD(distanceToCliff,3,0);
    }
    else if (patternStage == 1){
        RTC_MOVE_PATTERN_COUNTER = 0;
        RTC_FLAG_MOVE_PATTERN = 0;
        moveDistanceBackwards(distanceToCliff/10);
        LED0 = !LED0;
        patternStage++;
    }
    else if (patternStage == 2 && RTC_FLAG_MOVE_PATTERN){
        LED0 = !LED0;
        RTC_FLAG_MOVE_PATTERN = 0;
        
        return 1;
    }
    return 0;
}

char navigateMazePattern(char distance, int degrees)
{
    if(degrees == 0 && patternStage == 0){
            patternStage = 1;
            RTC_FLAG_MOVE_PATTERN = 1;
    }
    if(patternStage == 0){
        turning = 1;
        movingStraight = 0;
        if(degrees<0){
            turnDegreesCCW(-degrees-turnBoost);
        }
        else{
            turnDegreesCW(degrees+turnBoost);
        }
        turnBoost = 0;
        patternStage ++;
        
        RTC_MOVE_PATTERN_COUNTER = 0; 
        //Reset Pattern Flag
        RTC_FLAG_MOVE_PATTERN = 0;
        enteredFollowStage = 1;
    }
    else if (RTC_FLAG_MOVE_PATTERN&&patternStage == 1){
        enteredFollowStage = 1;
        turning = 0;
        movingStraight = 1;
        
        moveDistanceForward(distance*3/10);
        //increment pattern stage
        patternStage++;
        //Reset Pattern Flag
        RTC_FLAG_MOVE_PATTERN = 0;
    }
    else if (RTC_FLAG_MOVE_PATTERN&&patternStage == 2){
        //Make sure no wall follow the first part after cliff
        justLeftCliff = 0;
        enteredFollowStage = 1;
        turning = 0;
        movingStraight = 1;
        
        moveDistanceForward(distance*2/10);
        //increment pattern stage
        patternStage++;
        //Reset Pattern Flag
        RTC_FLAG_MOVE_PATTERN = 0;
    }
    else if (RTC_FLAG_MOVE_PATTERN&&patternStage == 3){
        turning = 0;
        movingStraight = 1;
        enteredFrontStage = 1;
        moveDistanceForward(distance*5/10);
        //increment pattern stage
        patternStage++;
        //Reset Pattern Flag
        RTC_FLAG_MOVE_PATTERN = 0;
    }
    else if (RTC_FLAG_MOVE_PATTERN&&patternStage == 4){
        //increment pattern stage
        patternStage = 0;
        //Reset Pattern Flag
        RTC_FLAG_MOVE_PATTERN = 0;
        return 1;
    }
    //Return 0 if pattern is not over
    return 0;
}
//-----MOVE PATTERNS END-----

//-----SENSOR READINGS START----
// Get the distance
int getTraveledDistance(){
    int distance = 0;
    ser_putch(142); //Set to read sensors
    __delay_ms(5);
    ser_putch(19); //Set drive packet ID
    __delay_ms(5);
    distance = ser_getch()*256;
    __delay_ms(5);
    distance += ser_getch(); //Combine high bit with low bit 
    return distance;
}
// Get the bump/drop sensor readings
char getBumpDropSensor(){
    char bumpSensor = 0;
    ser_putch(142); //Set to read sensors
    __delay_ms(5);
    ser_putch(7); //Set Bump/Drop packet ID
    __delay_ms(5);
    bumpSensor = ser_getch();
    return bumpSensor;
}
char getForceField(){
    ser_putch(142);//Set to read sensors
     __delay_ms(5);
    ser_putch(17); // Set packet id to infrared Byte
     __delay_ms(5);
     char result = ser_getch();
     if (result == 242||result == 252 || result == 250 || result == 246 ||result == 254){
         return 1;
     }
     return 0;
    
}
char getVirtualWall(){
    ser_putch(142);//Set to read sensors
     __delay_ms(5);
    ser_putch(13); // Set packet id to infrared Byte
     __delay_ms(5);
     return ser_getch();
    
}
//Get the cliff sensor readings
char getCliffSensors(){
    char cliffSensors = 0;
    //Cliff left sensor
    ser_putch(142); //Set to read sensors
    __delay_ms(5);
    ser_putch(9); //cliff front left
    __delay_ms(5);
    cliffSensors = cliffSensors | ser_getch();
    //Front left sensor
    ser_putch(142); //Set to read sensors
    __delay_ms(5);
    ser_putch(10); 
    __delay_ms(5);
    cliffSensors = cliffSensors | ser_getch();
    //Front Right sensor
    ser_putch(142); //Set to read sensors
    __delay_ms(5);
    ser_putch(11); 
    __delay_ms(5);
    cliffSensors  = cliffSensors | ser_getch();
    //Cliff right sensor
    ser_putch(142); //Set to read sensors
    __delay_ms(5);
    ser_putch(12); 
    __delay_ms(5);
    cliffSensors = cliffSensors | ser_getch();
    return cliffSensors;
}
//Function to update the sensors and see if the robot needs to stop
void updateSensors(){
    if(updateSensorsFlag){
        //Check bumpWheelSensors
        
        //char bumpSensor = getBumpDropSensor();
        // Check cliff sensors
        //char cliffSensors = getCliffSensors();
        //char bumpSensorResult = bumpSensor&0b00011111;
        char forceField = getForceField();
        
        
        if(forceField){
            LED0 = !LED0;
            //Stop all movement
            //stopAllPatterns();
           
            char mapSeg = readMapSegment(currentX, currentY);
            char prevMapSeg = readMapSegment(prevX, prevY);
             lcdSetCursor(0x06);
             char checkThis = 0b00010000;
             char hasChecked = checkThis & mapSeg;
             char hasCheckedPrev = checkThis & prevMapSeg;
             /*
            lcdWriteToDigitBCD(checkThis, 2,0);
            lcdSetCursor(0x08); 
            lcdWriteToDigitBCD(mapSeg, 2,0);
            lcdSetCursor(0x0A); 
            lcdWriteToDigitBCD(hasChecked, 2,0);
             */
            if(!hasChecked&&!hasCheckedPrev){
                writeMapSegment(currentX, currentY, mapSeg|checkThis);
                
                victimsFound++;
                if(victimsFound == 1){
                    playSong1();
                }
                else if(victimsFound == 2){
                    playSong2();
                }
                lcdSetCursor(0x0D);
                lcdWriteToDigitBCD(victimsFound, 1,0);
            }
            // Set the victim flag in map segment
            
            
        }
        //Update and write distance travelled
        /*
        distanceTraveled += getTraveledDistance();
        lcdSetCursor(0x40);
        lcdWriteToDigitBCD(distanceTraveled/10, 4, 0);
        */
        updateSensorsFlag = 0;
    }
}


//-----SENSOR READINGS END-----