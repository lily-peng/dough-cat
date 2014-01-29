#include "myLib.h"

unsigned short *videoBuffer = (u16 *)0x6000000;

void setPixel(int row, int col, u16 color)
{
	videoBuffer[OFFSET(row, col)] = color;
}
/*
void drawRect(int row, int col, int height, int width, u16 color)
{
	int r,c;
	for(r=0; r<height; r++)
	{
		for(c=0; c<width; c++)
		{
			setPixel(row+r, col+c, color);
		}
	}
}
*/


void drawRect(int row, int col, int height, int width, volatile u16 color)
{
	int r;
	for(r=0; r<height; r++)
	{
		DMA[3].src = &color;
		DMA[3].dst = videoBuffer + OFFSET(row+r, col);
		DMA[3].cnt = DMA_ON | DMA_SOURCE_FIXED | width;	
	}
}

void waitForVblank()
{
	while(SCANLINECOUNTER > 160);
	while(SCANLINECOUNTER < 160);
}
	
	
void fillScreen(volatile u16 color)
{	
	DMA[3].src = &color;
	DMA[3].dst = videoBuffer;
	DMA[3].cnt = DMA_ON | DMA_SOURCE_FIXED | 38400;	
}




