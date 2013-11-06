#include "ADC.H"
#include "UART.h"
#include "SystemClock.h"
#include "DACTimer.h"
#include "SSI.h"


struct SSI_module *ssi;

static BYTE_ADDRESS ADC0 = ((BYTE_ADDRESS)0x40038000); 
static BYTE_ADDRESS TIMER1 = ((BYTE_ADDRESS)0x40031000);

volatile unsigned int ADCdata = 0;
volatile unsigned int averageADCvalue = 0;
volatile unsigned int sumADCvalue = 0;
volatile unsigned int ADCcount = 0;
volatile double newTimerValue = 0;
volatile unsigned int newFreqValue = 0;

static const int sineWave[40] = {512,592,670,744,813,874,926,968,998,1017,1023,1017,998,968,926,874,813,744,670,592,512,432,354,280,211,150,98,56,26,7,0,7,26,56,98,150,211,280,354,432};
volatile unsigned char sineCount = 0;	
	
void ADC0Seq0_Handler(void);
unsigned char* short2charArray(unsigned int frequency);
void Timer1A_Handler(void);
	
int main(void)
{
	ssi = malloc(sizeof(*ssi));
	ssi = new_SSIMasterModule(0, FREESCALE, 1000000, 16, false, true, true);
	setSysClkTo_80MHz();
	
	initADC02msWithInterrupt();
	initTimerForDacOutput();
	
	while(1)
	{		
					
	}		
}

void ADC0Seq0_Handler(void)
{	
	unsigned char* string;
	ADCdata = read_word(ADC0, 0x048); // get adc data
	ADCcount++;
	sumADCvalue += ADCdata;
	averageADCvalue = sumADCvalue/ADCcount;
	
	if(ADCcount == 250)
	{		
		newTimerValue = (4.39453125)*averageADCvalue + 2000;
		newFreqValue = (-0.045)*newTimerValue + 1000;
		ADCcount = 0;
		averageADCvalue = 0;
		sumADCvalue = 0;
		// write new timer load value
		updateCount((int)(newTimerValue));
		// write new frequency to LCD
		string = short2charArray(newFreqValue);
		// clearText();
		// println(string,7);
	}
			
	write_word(ADC0, 0x00C, 0xFF); // clear interrupt
}

unsigned char* short2charArray(unsigned int frequency)
{
	unsigned char charArray[7];
	unsigned char* string = charArray;
	unsigned char temp = 0;
	
	temp  = frequency/1000;
	charArray[0] = temp + 0x30;
	frequency = frequency % 1000;
	
	temp = frequency/100;
	charArray[1] = temp + 0x30;
	frequency = frequency%100;
	
	temp = frequency/10;
	charArray[2] = temp + 0x30;
	frequency = frequency%10;
	
	charArray[3] = frequency + 0x30;
	
	charArray[4] = 0x20;
	charArray[5] = 0x48;
	charArray[6] = 0x7A;	
	
	return string;
}

void Timer1A_Handler(void)
{
	// send data to DAC
	SSI_sendData(ssi, ((sineWave[sineCount]<<2)&0xFFC));
	sineCount++;
	if(sineCount == 40)
		sineCount = 0;	
	// clear interrupt
	write_word(TIMER1, 0x024, 0xFFFFF);
	
}