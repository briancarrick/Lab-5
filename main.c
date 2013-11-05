#include "LCD.H"
#include "TouchScreen.h"
#include "SystemClock.h"

typedef struct{
	unsigned _1:1;
	unsigned _2:1;
	unsigned _3:1;
}Box;

//****************************************************************
// Define Globals
volatile int xPos = 0;
volatile int yPos = 0;
volatile int xTot = 0;
volatile int yTot = 0;
volatile int count = 0;
volatile char button = 0;
volatile Box boxes;
//****************************************************************

//void GPIOPortF_Handler(void);
void clickAction(void);

int main(void)
{
	setSysClkTo_80MHz();
	lcdPortConfig();
	lcdInit();
	TOUCH_initialize('f', 4, 1);
	
	clearLCD(black);
	
	
	while(1)
	{		
		if(boxes._1)
		{
			makeOpenBox(220,80,yellow);
		}
		else
		{
			makeBox(220,80,yellow);	
		}
		if(boxes._2)
		{
			makeOpenBox(120,80,green);
		}
		else
		{
			makeBox(120,80,green);
		}
		if(boxes._3)	
		{
			makeOpenBox(20,80,red);
		}
		else
		{
			makeBox(20,80,red);
		}
			
	}		
}

//void GPIOPortF_Handler(void)
//{
//	
//	int tempX = 0;
//	int tempY = 0;
//	count = 0;
//	xPos = 0;
//	yPos = 0;
//	xTot = 0;
//	yTot = 0;
//	// clear interrupt
//	PORTF[0x41C] = 0x10;
//	PORTF[0x404] = 0x00;
//	// disable interrupt
//	INTERRUPT[0x280/4] = 0x40000000;
//	INTERRUPT[0x180/4] = 0x40000000;
//	//INTERRUPT[0x280/4] = 0x40000000;
//	
//	
//	
//	// get x and y value
//	do{
//		SPI1[0x008] = 0x90; // get X position
//		SPI1[0x008] = 0x00;
//		SPI1[0x008] = 0x00;
//		SPI1[0x008] = 0xD0; // get Y position	
//		SPI1[0x008] = 0x00;
//		SPI1[0x008] = 0x00;
//		delayUS(30); // give microcontroller time to send and recieve bytes
//		tempX = SPI1[0x008];
//		tempX = (SPI1[0x008] << 5);
//		tempX |= (SPI1[0x008] >> 3);
//		tempY = SPI1[0x008];
//		tempY = (SPI1[0x008] << 5); 
//		tempY |= (SPI1[0x008] >> 3);
//		// average x and y values
//		count++;
//		xTot += tempX;
//		yTot += tempY;
//		xPos = xTot/count;
//		yPos = yTot/count;
//	} while(!(PORTF[0x3FC] & 0x10)); //check for PF.4 high
//	clickAction();
//	delayUS(3250);
//	// enable interrupts
//	INTERRUPT[0x280/4] = 0x40000000; // clear any pending interrupts
//	INTERRUPT[0x100/4] = 0x40000000; // re-enable gpioF interrupts
//	
//	PORTF[0x41C] = 0x10; // clear any pending interrupts
//	PORTF[0x404] = 0x10; // re-enable GPIOF.4 interrupts
//	
//}

/////////////////////////////////////////////////////////////////////////
// clickAction
// Check for touchscreen button press
// Inputs:
//		xPos - average x position of last touch
//		yPos - average y position of last touch
// Outputs:
//		NONE
/////////////////////////////////////////////////////////////////////////
void clickAction(void)
{
	if(yPos >= 1364 && yPos <= 2732)
	{
		if(xPos >= 356 && xPos <=1380)
			boxes._1 = ~boxes._1;
		else if(xPos >= 1536 && xPos <=2560)
			boxes._2 = ~boxes._2;
		else if(xPos >= 2816 && xPos <= 3840)
			boxes._3 = ~boxes._3;
	}
}
// End clickAction ///////////////////////////////////////////////////////////
