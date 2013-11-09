#include "Screen.h"
#ifdef LANDSCAPE
	#define HEIGHT 30
	#define WIDTH 40
#else
	#define HEIGHT 40
	#define WIDTH 30
#endif
#define CHAR_WIDTH 8
#define CHAR_HEIGHT 8


const HALF_WORD ColorTable[] = {0xFFFF,0x0000,0xF7DE,0x001F,0xF800,0xF81F,0x07E0,0x7FFF,0xFFE0};

static struct {
	WORD curLoc;
	BYTE charSpace[HEIGHT*WIDTH];
	
	WORD yOffset;
	struct font_module font;
} Screen;

void startScreen(void){
	lcdInit();
	Screen.curLoc = 0;
	Screen.font.background = getColorCode(black);
	Screen.font.foreground = getColorCode(green);
	Screen.font.fontType = Font8x8;
	Screen.font.height = CHAR_HEIGHT;
	Screen.font.width = CHAR_WIDTH;
}
HALF_WORD getPixelYCoordinate(WORD position){
	return (position/WIDTH)*CHAR_HEIGHT;
}
HALF_WORD getPixelXCoordinate(WORD position){
	return (position%WIDTH)*CHAR_WIDTH;
}
struct Vector2* getPixelCoordinate(WORD position){
	struct Vector2* retVal = malloc(sizeof *retVal);
	retVal->x = getPixelXCoordinate(position);
	retVal->y = getPixelYCoordinate(position);
	return retVal;
}
WORD startOfNextLine(WORD position){
	return (position+WIDTH)-(position%WIDTH);
}
WORD startOfThisLine(WORD position){
	if(position%WIDTH == 0)
		position--;
	return position - (position%WIDTH);
}
void blankOut(WORD startPosition, WORD endPosition){
	WORD i = 0;
	struct Vector2* startPxCoordinate;
	struct Vector2* boxSize;
	WORD numLines = (endPosition/WIDTH) - (startPosition/WIDTH);
	if(startPosition > endPosition)
		swap(startPosition, endPosition);
	for(; i < numLines;i++){
		startPxCoordinate = getPixelCoordinate(startPosition);
		if(endPosition < startOfNextLine(startPosition)){
			boxSize = new_Vector2
				((getPixelXCoordinate(endPosition)+CHAR_WIDTH)-startPxCoordinate->x
				,CHAR_HEIGHT
				);
		} else {
			boxSize = new_Vector2
				((getPixelXCoordinate((startOfNextLine(startPosition)-1))+CHAR_WIDTH)-startPxCoordinate->x
				,CHAR_HEIGHT
				);
			startPosition = startOfNextLine(startPosition);
		}
		makeBox(startPxCoordinate, boxSize, Screen.font.background); 
		free(startPxCoordinate);
		free(boxSize);
	}
}
void printChar(BYTE c, WORD position){
	struct Vector2* coord = malloc(sizeof *coord);
	coord->x = (position%WIDTH)*8;
	coord->y = (position/WIDTH)*8;
	drawAsciiChar(c, coord, &Screen.font);
	free(coord);
}
//Globally Scoped Functions
void setTextColor(Color color){
	Screen.font.foreground = ColorTable[color];
}
void setBackgroundColor(Color color){
	Screen.font.background = ColorTable[color];
}

void clearText(void){
	int i = 0;
//	blankOut(0,Screen.curLoc);
	for(; i < Screen.curLoc; ++i)
	{
		if(Screen.charSpace[i]!=NULL)
			printChar(' ', i);
		Screen.charSpace[i] = 0;
	}
	Screen.curLoc = 0;
}

void println(char* string){
	int i = 0;
	for(;i<strlen(string);++i){
		if(string[i] == 0x0A){
			Screen.curLoc += WIDTH;
			Screen.curLoc -= (Screen.curLoc%WIDTH);
		}else{
			Screen.charSpace[Screen.curLoc] = string[i];
			printChar(string[i], Screen.curLoc);
			++Screen.curLoc;
		}
		if(Screen.curLoc >= WIDTH*HEIGHT)
			Screen.curLoc = 0;
	}
	Screen.curLoc = startOfNextLine(Screen.curLoc);
	if(Screen.curLoc >= WIDTH*HEIGHT)
		Screen.curLoc = 0;
}
void replaceLastLine(char* string){
	WORD i = 0;
	WORD curLoc;
	curLoc = i+startOfThisLine(Screen.curLoc);
	for(;i<WIDTH;++i){
		if(i<strlen(string)){
			if(Screen.charSpace[curLoc] != string[i])
				printChar(string[i], curLoc);
		}else if(Screen.charSpace[curLoc]!=0)
			printChar(' ', curLoc);
		curLoc++;
	}
	Screen.curLoc = startOfThisLine(Screen.curLoc);
}
HALF_WORD getColorCode(Color color){
	return ColorTable[color];
}
