
#ifndef XC_HEADER_TEMPLATE_H
#define	XC_HEADER_TEMPLATE_H

#include "common.h"
#include "scanner.h"
#include "iRobot.h"
#include "mapNav.h"
#include "eeprom.h"
#include "music.h"

    
char followWallPatternStart = 0;

char navigateMazePatternStart = 0;
char updateMap = 1;
int degreesToTurn = 0;
char wallFollowDirection = 1;

char prevWallFollowDirection = 0;
char noWalls = 0;

char resetDistanceReader = 1;
char movingToCliff = 0;
    
    //Check if the robot should scan for closest wall or scan and move to closest wall
char onlyScan = 1;
void updatePatterns();
void stopAllPatterns();

#endif	/* XC_HEADER_TEMPLATE_H */

