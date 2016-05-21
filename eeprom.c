/*
 * File:   eeprom.c
 * Author: JohnMac
 *
 * Created on den 21 maj 2016, 14:59
 */


#include "eeprom.h"


void setStartPos(char orient, char x, char y){
    orientation = orient; 
    currentX = x;
    currentY = y;
}
void writeMapSegment(char x, char y, char data){
    
    char address = (x-1)*MAPCOLUMNS + y;
    eeprom_write(address, data);
}
char readMapSegment(char x, char y){
    
    char address = (x-1)*MAPCOLUMNS + y;
    return eeprom_read(address);
}
void writeMapPriority(char x, char y, char data){
    
    char address = (x-1)*MAPCOLUMNS + y + MAPCOLUMNS*MAPROWS;
    eeprom_write(address, data);
}
char readMapPriority(char x, char y){
    
    char address = (x-1)*MAPCOLUMNS + y + MAPCOLUMNS*MAPROWS;
    return eeprom_read(address);
}
void writeAdvancedMap(){
    writeMapSegment(1,2,0b00001110); //1,2
    writeMapPriority(1,2,1);
    
    writeMapSegment(1,1,0b00001001); //1,1
    writeMapPriority(1,1,2);
    
    writeMapSegment(2,1,0b00000011); //2,1
    writeMapPriority(2,1,3);
    
    writeMapSegment(2,2,0b00001010); //2,2
    writeMapPriority(2,2,4);   
    
    writeMapSegment(2,3,0b00000000); //2,3
    writeMapPriority(2,3,5);
    
    writeMapSegment(1,3,0b00001101); //1,3
    writeMapPriority(1,3,6);
    
    writeMapSegment(3,3,0b00000101); //3,3
    writeMapPriority(3,3,6);
    
    writeMapSegment(4,3,0b00000010);
    writeMapPriority(4,3,7);
    
    writeMapSegment(4,2,0b00000011);
    writeMapPriority(4,2,8);
    
    writeMapSegment(1,4,0b00001011);
    writeMapPriority(1,4,13);
    
    writeMapSegment(3,2,0b00001100);
    writeMapPriority(3,2,9);
    
    writeMapSegment(3,1,0b00001001);
    writeMapPriority(3,1,10);
    
    writeMapSegment(4,1,0b00000111);
    writeMapPriority(4,1,11);
    
    writeMapSegment(4,4,0b00000010);
    writeMapPriority(4,4,8);
    
    writeMapSegment(3,4,0b00001101);
    writeMapPriority(3,4,9);
    
    writeMapSegment(4,5,0b00000110);
    writeMapPriority(4,5,9);
    
    writeMapSegment(3,5,0b00000101);
    writeMapPriority(3,5,10);
    
    writeMapSegment(2,5,0b00000100);
    writeMapPriority(2,5,11);
    
    writeMapSegment(2,4,0b00001010);
    writeMapPriority(2,4,12);
    
    writeMapSegment(1,5,0b00001100);
    writeMapPriority(1,5,12);
    
}


