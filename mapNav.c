
#include "mapNav.h"
#include "eeprom.h"
#include "lcd.h"
bit canMove(char direction){
    //Check if we are outside the maze
    if(direction == NORTH &&(northX>MAPROWS||northX<1||northY>MAPCOLUMNS||northY<1)){
        return 0;
    }
    if(direction == WEST &&(westX>MAPROWS||westX<1||westY>MAPCOLUMNS||westY<1)){
        return 0;
    }
    if(direction == EAST &&(eastX>MAPROWS||eastX<1||eastY>MAPCOLUMNS||eastY<1)){
        return 0;
    }
    if(direction == SOUTH &&(southX>MAPROWS||southX<1||southY>MAPCOLUMNS||southY<1)){
        return 0;
    }
        

	switch(direction){
		case NORTH:
			if (readMapSegment(northX,northY)&0b00000010) // If we want to turn north check for south wall in segment
				return 0;
		break;

		case WEST:
		if (readMapSegment(westX,westY)&0b00000100)	// If we want to turn west check for east wall in segment
				return 0;
		break;

		case SOUTH:
		if (readMapSegment(southX,southY)&0b00001000) // If we want to turn south check for north wall in segment
				return 0;
		break;

		case EAST:
		if (readMapSegment(eastX,eastY)&0b00000001) // If we want to turn east check for west wall in segment
				return 0;
		break;
	}
	return 1;
}

int turn(signed char direction){
    char newOrientation = 0;
    
    prevX = currentX;
    prevY = currentY;
    
    switch(direction){
        case NORTH:
            currentX = northX;
            currentY = northY;
            newOrientation = NORTH;
            break;
        case EAST:
            currentX = eastX;
            currentY = eastY;
            newOrientation = EAST;
            break;
        case SOUTH:
            currentX = southX;
            currentY = southY;
            newOrientation = SOUTH;
            break;
        case WEST:
            currentX = westX;
            currentY = westY;
            newOrientation = WEST;
            break;
          
    }
    
    int degrees = (direction-orientation)*90;
    signed char specialCaseTest = (direction-orientation);

    if(specialCaseTest == 3){
        degrees = -90;
    }
    if(specialCaseTest == -3){
        degrees = 90;
    }
    orientation = newOrientation;
    
    return degrees;
}
char findFastest(){
    char returnDirection = NORTH;
    char smallestPriority = 20;
    
    if(canMove(NORTH)&&readMapPriority(northX,northY)<smallestPriority){
        smallestPriority = readMapPriority(northX,northY);
        returnDirection = NORTH;
    }
    if(canMove(EAST)&&readMapPriority(eastX,eastY)<smallestPriority){
        smallestPriority = readMapPriority(eastX,eastY);
        returnDirection = EAST;
    }
    if(canMove(SOUTH)&&readMapPriority(southX,southY)<smallestPriority){
        smallestPriority = readMapPriority(southX,southY);
        returnDirection = SOUTH;
    }
    if(canMove(WEST)&&readMapPriority(westX,westY)<smallestPriority){
        smallestPriority = readMapPriority(westX,westY);
        returnDirection = WEST;
    }
    return returnDirection;
}
int  moveSegment(){
    
   

    northX = currentX; // The x coordinate to the segment north of the robot
    northY = currentY; // The y coordinate to the segment north of the robot

	westX = currentX;	// west segment x
	westY = currentY;// west segment y

	southX = currentX; // south segment x
	southY = currentY;	// south segment y

	eastX = currentX; // east segment x
    eastY = currentY;	// east segment y
	//Set the north, west, south, east segments around the robot 
	northX += -1; //2,2 ---> 1,2
	northY += 0;  

	westX += 0; //2,2 ---> 2,1
	westY += -1;

	southX += 1; //2,2 ---> 3,2
	southY += 0;

	eastX += 0;  //2,2 ---> 2,3
	eastY += 1;

	// Check oriantation and dicide where to move, use canMove function to check if that segment has a wall or not
	// Always move left if there are more than one option
    if(victimsFound == 2){
            LED0 = !LED0;
            char turnDirection = findFastest();
            return turn(turnDirection);
    } 
	switch(orientation){
		case NORTH:
			if(canMove(WEST)){
				return turn(WEST);
                
            }
			else if(canMove(NORTH))//Do nothing go straight
                return turn(NORTH);
			else if(canMove(EAST))
				return turn(EAST);
			else
				return turn(SOUTH); // Dead end go back
		break;

		case WEST:
			if(canMove(SOUTH)){
				return turn(SOUTH);
            }
			else if(canMove(WEST)){
                
				return turn(WEST);//Do nothing go straight
            }
			else if(canMove(NORTH)){
				return turn(NORTH);
            }
			else{
                
				return turn(EAST); // Dead end go back
            }
		break;

		case SOUTH:
			if(canMove(EAST))
				return turn(EAST);
			else if(canMove(SOUTH))
				return turn(SOUTH);//Do nothing go straight
			else if(canMove(WEST))
				return turn(WEST);
			else
				return turn(NORTH); // Dead end go back
		break;

		case EAST:
			if(canMove(NORTH))
				return turn(NORTH);
			else if(canMove(EAST))
				return turn(EAST);//Do nothing go straight
			else if(canMove(SOUTH))
				return turn(SOUTH);
			else
				return turn(WEST); // Dead end go back
		break;
	}
    return 0;
}

int getWallFollowDirection(char prev){
    int wallAt = 7;
    char x = 0;
    char y = 0;
    if(prev){
        x = prevX;
        y = prevY;
    }
    else{
        x = currentX;
        y = currentY;
    }
    if(orientation==NORTH||orientation==SOUTH){
        if(readMapSegment(x, y)&0b00000001){
            wallAt = WEST;
        }
        if(readMapSegment(x, y)&0b00000100){
            wallAt = EAST;
        }
    }
    if(orientation==WEST||orientation==EAST){
        if(readMapSegment(x, y)&0b00001000){
            wallAt = NORTH;
        }
        if(readMapSegment(x, y)&0b00000010){
            wallAt = SOUTH;
        }
    }
    
    int result = orientation-wallAt;
    //This is a algoritm to see if the robot shold turn right or left, 1 is right, 0 is left
    if(result == -1 || result == 3){
        return 1;
    }
    else if(result == 1 || result == -3){
        return 0;
    }
    //If no wall found return 2
    return 2;
}
void printPosition(char x, char y, char dir){
    lcdSetCursor(0x40);
    lcdWriteString("X:");
    lcdWriteToDigitBCD(x,1,0);
            
    lcdSetCursor(0x44);
    lcdWriteString("Y:");
    lcdWriteToDigitBCD(y,1,0);
     
    lcdSetCursor(0x48);
    if(dir == EAST){
        lcdWriteString("EAST");
    }
    else if(dir == WEST){
        lcdWriteString("WEST");
    }
    else if(dir == SOUTH){
        lcdWriteString("SOUTH");
    }
    else if(dir == NORTH){
        
        lcdWriteString("NORTH");
    }
}



