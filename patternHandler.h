
#ifndef XC_HEADER_TEMPLATE_H
#define	XC_HEADER_TEMPLATE_H

#include "common.h"
#include "scanner.h"
#include "iRobot.h"

char patternDone = 1;
char squarePatternDone = 1;
char straightPatternDone = 1;
char moveToWallPatternDone = 1;
    
char followWallPatternStart = 0;
    
    //Check if the robot should scan for closest wall or scan and move to closest wall
char onlyScan = 1;
void updatePatterns();
void stopAllPatterns();

#endif	/* XC_HEADER_TEMPLATE_H */

