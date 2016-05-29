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
    addTone('E',3, 0, 10);
    
    addTone('E',3, 0, 20);
    addTone('E',3, 0, 20);
    
    addTone('C',3, 0, 10);
    addTone('E',3, 0, 20);
    
    addTone('G',3, 0, 40);
    addTone('G',2, 0, 40);
     


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
    addTone('A',3, 0, 30); //addTone('F',3, 0, 10); Skip this one
    addTone('G',3, 0, 20);
    
    addTone('E',3, 0, 20);
    addTone('C',3, 0, 10);
    addTone('D',3, 0, 10);
    addTone('D',3, 0, 10);
    
}

void setupMarioStarTheme(){
    startSetSong(16, 3);//Set song nr 3, Song length 16
    addTone('C',3, 0, 10);
    addTone('C',3, 0, 10);
    addTone('C',3, 0, 5);
    addTone('C',3, 0, 10);
    addTone('C',3, 0, 10);
    addTone('C',3, 0, 15);
    //addTone('C',3, 0, 5); Removed to make 16 notes...
    addTone('C',3, 0, 10);
    addTone('C',3, 0, 10);
    
    addTone('B',2, 0, 10);
    addTone('B',2, 0, 10);
    addTone('B',2, 0, 5);
    addTone('B',2, 0, 10);
    addTone('B',2, 0, 10);
    addTone('B',2, 0, 15);
    //addTone('B',2, 0, 5); Removed to make 16 notes...
    addTone('B',2, 0, 10);
    addTone('B',2, 0, 10);

}
void setupLevelCompleteTheme(){
    startSetSong(16, 4);//Set song nr 4, Song length 16
    addTone('F',3, 0, 20);
    addTone('F',3, 0, 20);
    addTone('D',3, 0, 10);
    addTone('F',3, 0, 30);
    //addTone('D',3, 0, 10);
    
    addTone('F',3, 0, 10);
    addTone('D',3, 0, 20);
    //addTone('C',3, 0, 10);
    addTone('F',3, 0, 50);
    //addTone('D',3, 0, 10);
    
    addTone('C',4, 0, 10);
    addTone('D',4, 0, 10);
    addTone('C',4, 0, 10);
    addTone('D',4, 0, 10);
    addTone('C',4, 0, 20);
    
    //addTone('C',3, 0, 5);
    addTone('A',3, 1, 5);
    addTone('A',3, 0, 5); 
    addTone('G',3, 0, 10);
    
    addTone('F',3, 0, 40);
}

void setupSongs(){
    setupMarioThemePart1();
    setupMarioThemePart2();
    setupLevelCompleteTheme();
    setupMarioStarTheme();
}

void playSong1(){
       ser_putch(141); __delay_ms(5); ser_putch(1);
}
void playSong2(){
       ser_putch(141); __delay_ms(5); ser_putch(2);
}

void playSong3(){
    ser_putch(141); __delay_ms(5); ser_putch(3);
}
void playSong4(){
    ser_putch(141); __delay_ms(5); ser_putch(4);
}