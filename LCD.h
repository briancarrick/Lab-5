/* function prototypes for SSD1289 LCD on Tiva C Launchpad */

#ifndef LCD_H
#define LCD_H

#include "SSI.h"
#include "JLib.h"
#include "SYSTIC.h"

// 16-bit RGB values for some common colors
#define white   0xFFFF
#define black   0x0000
#define grey    0xF7DE
#define blue    0x001F
#define red     0xF800
#define magenta 0xF81F
#define green   0x07E0
#define cyan    0x7FFF
#define yellow  0xFFE0


// functions to init LCD and GPIO
void lcdPortConfig(void); //setup GPIO pins
void lcdInit(void); //base LCD config

// functions to send cmd/data to LCD
void writeCmd(unsigned short cmd); //command: where we write data to
void writeDat(unsigned short dat); //data: what gets written to address given by cmd
void writeReg(unsigned short cmd,unsigned short dat); //either wrapper for above or combine above to avoid function calls

// helper functions
void clearLCD(unsigned short rgb); //set entire LCD to the color rgb (useful for debugging above)
void setCursor(unsigned short x,unsigned short y); //set current pixel to x,y
void setCursorIndex(unsigned int index); //set current pixel to ind (use index to address pixel instead of x,y)

// functions to draw boxes
void makeBox(unsigned short x,unsigned short y,unsigned short rgb); //create a solid box @x,y of color given by rgb
void makeOpenBox(unsigned short x,unsigned short y,unsigned short rgb); //create outline of box @x,y of color given by rgb

#endif
