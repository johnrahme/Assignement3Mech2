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

//This pattern makes the robot follow the wall, version 2 uses drive direct and is smoother than version 1
char followWallPatternV2(){
    //Check if the move pattern flag has been raised
    if(patternStage == 0|| (patternStage == 1 && RTC_FLAG_MOVE_PATTERN)){
        //Reset counter
        RTC_MOVE_PATTERN_COUNTER = 0; 
        MOVE_PATTERN_TIME = 10; //How often to update
        int valueOff = latestReadMeterValue-50;
        valueOff*10; // Convert To millimeters
        int speedRightWheel = 0;
        int speedLeftWheel = 0;
        char divideBy = 1;
        char times = 1;
        
        //If the distance is far make the divideBy factor bigger so as not to turn to fast
        if((valueOff>80)){
            divideBy = 14; 
        }
        //If the robot is really close to the edge turn really fast
        else if(valueOff<-5){
            times = 15;
        }
        //Check if the boost has been activated whenever the wall has been lost
        if(boostActivated&&valueOff>10){
            
            times = times*6;
        }
        //Set the wheel speed
         speedRightWheel = 200+valueOff/divideBy*times;
         speedLeftWheel = 200-valueOff/divideBy*times;
        
         //Check if it lost the wall
         
         //Check if the sensor has lost the wall and raise lost wall flag
        if(lastValueOff < 10 && valueOff > 20 && !activateLostWall){
            
            activateLostWall = 1;
            RTC_LOST_WALL_FLAG = 0;
            RTC_LOST_WALL_COUNTER = 0;
            LOST_WALL_TIME = 4000;
        }
        //If the lost wall timer is done, turn fast
        if(RTC_LOST_WALL_FLAG&&activateLostWall){
            //If the lost wall flag has been raised activate boost
            RTC_LOST_WALL_FLAG = 0;
           if(lostWallCounter == 0){
               LED0 = !LED0;
               lostWallCounter++;
               boostActivated = 1;
           }
            //When the lost wall flag is raised the second time deactivate boost
           else{
               LED0 = !LED0;
               activateLostWall = 0;
               boostActivated = 0;
               lostWallCounter = 0;
           }       
        }
        lastValueOff = valueOff;
        //Use turn and drive direct functions
        turnAndDriveDirect(speedRightWheel,speedLeftWheel);
        //Do not increment patternStage since we want this function to run forever, unless its stage 0
        if(patternStage == 0){
             patternStage++;
        }
       
        //Reset Pattern Flag
        RTC_FLAG_MOVE_PATTERN = 0;
    }
}

// The old way of following the wall, not used in presentation. Can be ignored.
char followWallPattern(){
    if(patternStage == 0){
        RTC_MOVE_PATTERN_COUNTER = 0; 
        MOVE_PATTERN_TIME = 20;
        int valueOff = latestReadMeterValue-50;
        valueOff*10; // Convert To millimeters
        int radius = 0;
        radius = 20000/valueOff;
        
        turnAndDrive(radius);
        //increment pattern stage
        patternStage++;
        //Reset Pattern Flag
        RTC_FLAG_MOVE_PATTERN = 0;
    }
    else if(patternStage == 1 && RTC_FLAG_MOVE_PATTERN){
        RTC_MOVE_PATTERN_COUNTER = 0; 
        MOVE_PATTERN_TIME = 20;
        int valueOff = latestReadMeterValue-50;
        valueOff*10; // Convert To millimeters
        int radius = 0;
        radius = 20000/valueOff;
        
        turnAndDrive(radius);
        //Do not increment patternStage since we want this function to run forever
        //patternStage++;
        //Reset Pattern Flag
        RTC_FLAG_MOVE_PATTERN = 0;
    }
}
//Move towards the wall and then turn 90 degrees
char moveTowardsWallPattern(int degree, int distance)
{
    //Turn back to face the wall
    if(patternStage == 0){
        //turn a specified amount of degrees
        turnDegreesCCW(degree);
        //increment pattern stage
        patternStage++;
        //Reset Pattern Flag
        RTC_FLAG_MOVE_PATTERN = 0;
    }
    //Move towards the wall
    if(patternStage == 1&&RTC_FLAG_MOVE_PATTERN){
        if(distance > 15){
            moveDistanceForward(distance);
            //Reset Pattern Flag
            RTC_FLAG_MOVE_PATTERN = 0;
        }
        patternStage++;
    }
    //Turn 90 degrees
    if(patternStage == 2&&RTC_FLAG_MOVE_PATTERN){
        
        turnDegreesCW(90);
        //increment pattern stage
        patternStage++;
        //Reset Pattern Flag
        RTC_FLAG_MOVE_PATTERN = 0;
    }
    //Stops
    else if (RTC_FLAG_MOVE_PATTERN&&patternStage == 3){
        //End of pattern stop the motor
        stop();
        //Reset pattern stage
        patternStage = 0;
        RTC_FLAG_MOVE_PATTERN = 0;
        
        // Return 1 to show that pattern is over
        return 1;
    }
    //Return 0 if pattern is not over
    return 0;
}

//Pattern for moving straight 400 meters
char moveStraightPattern()
{
    if(patternStage == 0){
        //Move forward 400 cm
        moveDistanceForward(400);
        //increment pattern stage
        patternStage++;
        //Reset Pattern Flag
        RTC_FLAG_MOVE_PATTERN = 0;
    }
    else if (RTC_FLAG_MOVE_PATTERN&&patternStage == 1){
        //End of pattern stop the motor
        stop();
        //Reset pattern stage
        patternStage = 0;
        RTC_FLAG_MOVE_PATTERN = 0;
        

        // Return 1 to show that pattern is over
        return 1;
    }
    //Return 0 if pattern is not over
    return 0;
}

// Move Square pattern
char moveSquarePattern(){
    
    // Pattern stage 0, 2, 4 and 6 should move forward
    if(patternStage == 0||(RTC_FLAG_MOVE_PATTERN&&patternStage == 2)||(RTC_FLAG_MOVE_PATTERN&&patternStage == 4)||(RTC_FLAG_MOVE_PATTERN&&patternStage == 6)){
        //Move forward 100 cm
        moveDistanceForward(100);
        //increment pattern stage
        patternStage++;
        //Reset Pattern Flag
        RTC_FLAG_MOVE_PATTERN = 0;
    }
    // Pattern stage 1, 3, 5, and 7 should turn the robot
    else if((RTC_FLAG_MOVE_PATTERN&&patternStage == 1)||(RTC_FLAG_MOVE_PATTERN&&patternStage == 3) || (RTC_FLAG_MOVE_PATTERN&&patternStage == 5)|| (RTC_FLAG_MOVE_PATTERN&&patternStage == 7)){
        //Turn 0+ degrees
        turnDegreesCW(90);
        //increment pattern stage
        patternStage++;
        //Reset Pattern Flag
        RTC_FLAG_MOVE_PATTERN = 0;
    }
    // Stop the robot
    else if (RTC_FLAG_MOVE_PATTERN&&patternStage == 8){
        //End of pattern stop the motor
        stop();
        //Reset pattern stage
        patternStage = 0;
        RTC_FLAG_MOVE_PATTERN = 0;
        
        // Return 1 to show that pattern is over
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
        
        char bumpSensor = getBumpDropSensor();
        // Check cliff sensors
        char cliffSensors = getCliffSensors();
        char bumpSensorResult = bumpSensor&0b00011111;
        char forceField = getForceField();
        
        
        if(bumpSensorResult||cliffSensors ||forceField || getVirtualWall()){
            LED0 = !LED0;
            //Stop all movement
            stopAllPatterns();
            //playSong();
            
        }
        //Update and write distance travelled
        distanceTraveled += getTraveledDistance();
        lcdSetCursor(0x40);
        lcdWriteToDigitBCD(distanceTraveled/10, 4, 0);
        updateSensorsFlag = 0;
    }
}


//-----SENSOR READINGS END-----