#ifndef SPI_H
#define	SPI_H
#include "common.h"

#define SM_Control_Byte SSPBUF	//SM_Control_Byte is SSPBUF

//Macros that you can call for ease of use
#define     SELECT_NONE()	RC0 = 0; RC1 = 0;
#define     SELECT_EEPROM()	RC0 = 0; RC1 = 1;
#define     SELECT_RF()		RC0 = 1; RC1 = 0;
#define     SELECT_SM()		RC0 = 1; RC1 = 1;
#define     SM_STEP()		RC2 = 1; NOP(); RC2 = 0;


volatile unsigned int time_count;
volatile bit FLAG_1000MS;
char currentDirection = 1;
//Function prototypes
void setupSPI(void);
unsigned char spi_transfer(unsigned char data);
void setToMotorCW(void);
void setToMotorCCW(void);



#endif	/* SPI_H */

