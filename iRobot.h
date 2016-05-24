/* 
 * File:   iRobot.h
 * Author: JK
 *
 * Created on 21 April 2016, 2:21 PM
 */

#ifndef IROBOT_H
#define	IROBOT_H

#include "common.h"
#include "ser.h"
#include "timer0.h"
#include "lcd.h"
#include "adConv.h"
#include "patternHandler.h"
#include "music.h"

#define		START                   128			//Create start command
#define		FULL			132			//Create full control command
#define		DRIVE			137			//Create Drive command
#define		BUTTONS			18			//state of the create push buttons
#define		LEDS			139			//control the state of the create leds
#define		IRBYTE			17			//IR byte recieved by create
#define		SONG_PLAYING            37			//indicates if a song is currently being played
#define		PLAY_SONG		141			//play a song (0 - 15)
#define		SONG			140			//define a song
#define     DRIVE_DIRECT    145         //Set drive direct
int distanceTraveled = 0;

void setupIRobot(void);
void drive(void);
void stop(void);
char moveSquarePattern(void);
void turnCW(void);
void turnCCW(void);
void turnDegreesCCW(int degrees);
void turnAndDrive(int radius);
int getTraveledDistance(void);
void updateSensors(void);
char moveTowardsWallPattern(int degree, int distance);
void turnAndDriveDirect(int rightVelocity, int leftVelocity);

char followWallPattern();
char followWallPatternV2();

char navigateMazePattern(char distance, int degrees);

char patternStage = 0;
int lastValueOff = 0;
char activateLostWall = 0;
char lostWallCounter = 0;
char turning = 0;
char movingStraight = 0;

char followPatternStage = 0;
bit boostActivated = 0;

#endif	/* IROBOT_H */

