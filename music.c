/*
 * File:   music.c
 * Author: JohnMac
 *
 * Created on den 3 maj 2016, 16:28
 */


#include "music.h"

void startSetSong(char length, char nr){
    ser_putch(140); __delay_ms(5); ser_putch(nr); __delay_ms(5);ser_putch(length); __delay_ms(5);
}
void addTone(char tone, char octave, char sharp, char length){
    char baseTone = 0;
    switch(tone){ 
        case 'C':
            baseTone = 36;
            break;
        case 'D':
            baseTone = 38;
            break;
        case 'E':
            baseTone = 40;
            break;
        case 'F':
            baseTone = 41;
            break;
        case 'G':
            baseTone = 43;
            break;         
        case 'A':
            baseTone = 45;
            break;
        case 'B':
            baseTone = 47;
            break; 
    }
    //Check if the tone should be increased by half a note
    if (sharp)
        baseTone++;
    //add the octave
    baseTone += octave*12;
    ser_putch(baseTone); __delay_ms(5); ser_putch(length); __delay_ms(5);
}
void setupMarioThemePart1(){
    startSetSong(7, 1);//Set song nr 1, Song length 7
//    ser_putch(76); __delay_ms(5); ser_putch(10); __delay_ms(5);//E5 for 8/64 second
//    ser_putch(76); __delay_ms(5); ser_putch(20); __delay_ms(5);//E5 for 16/64 second
//    ser_putch(76); __delay_ms(5); ser_putch(20); __delay_ms(5);//E5 for 16/64 second
//    
//    ser_putch(72); __delay_ms(5); ser_putch(10); __delay_ms(5);//C5 for 8/64 second
//    ser_putch(76); __delay_ms(5); ser_putch(20); __delay_ms(5);//E5 for 16/64 second
//    ser_putch(79); __delay_ms(5); ser_putch(40); __delay_ms(5);//G5 for 32/64 second
//    
//    ser_putch(67); __delay_ms(5); ser_putch(20); __delay_ms(5);//G4 for 32/64 second
    
    //Part one
    addTone('E',3, 0, 10);
    addTone('E',3, 0, 20);
    addTone('E',3, 0, 20);
    addTone('C',3, 0, 10);
    addTone('E',3, 0, 20);
    
    addTone('G',3, 0, 40);
    addTone('G',2, 0, 40);
    
    
    //Part 2

    


}

void setupMarioThemePart2(){
    startSetSong(16, 2);
    
    addTone('C',3, 0, 30);
    addTone('G',2, 0, 30);
    addTone('E',2, 0, 30);
    
    addTone('A',2, 0, 20);
    addTone('B',2, 0, 20);
    addTone('A',2, 1, 10);
    addTone('A',2, 0, 20);
    
    addTone('G',2, 0, 13);
    addTone('E',3, 0, 13);
    addTone('G',3, 0, 14);
    addTone('A',3, 0, 20);
    addTone('F',3, 0, 10);
    addTone('G',3, 0, 20);
    
    addTone('E',3, 0, 20);
    addTone('C',3, 0, 10);
    addTone('D',3, 0, 10);
    
    //One more needed
    
}

void setupSongs(){
    setupMarioThemePart1();
    setupMarioThemePart2();
}

void playSong(){
       ser_putch(141); __delay_ms(5); ser_putch(2);
}