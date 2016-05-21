 
#ifndef EEPROM_H
#define	EEPROM_H
#define MAPCOLUMNS 5
#define MAPROWS 4
#include "common.h"

void setStartPos(char orient, char x, char y);

void writeMapSegment(char x, char y, char data);
char readMapSegment(char x, char y);
void writeMapPriority(char x, char y, char data);
char readMapPriority(char x, char y);

signed char orientation = 0; 
signed char currentX = 0;
signed char currentY = 0;

void writeAdvancedMap();

#endif	/*EEPROM_H */

