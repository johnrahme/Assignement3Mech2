
#ifndef MAPNAV_H
#define	MAPNAV_H
#include "common.h"


	signed char northX = 0; // The x coordinate to the segment north of the robot
	signed char northY = 0; // The y coordinate to the segment north of the robot

	signed char westX = 0;	// west segment x
	signed char westY = 0;// west segment y

	signed char southX = 0; // south segment x
	signed char southY = 0;	// south segment y

	signed char eastX = 0; // east segment x
	signed char eastY = 0;	// east segment y
    int  moveSegment();
    void printPosition(char x, char y, char dir);
    int getWallFollowDirection();

#endif	/* MAPNAV_H */

