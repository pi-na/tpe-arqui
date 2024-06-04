#ifndef NAIVE_CONSOLE_H
#define NAIVE_CONSOLE_H
#define GMT -3
#include <stdint.h>

typedef enum
{
	BLACK = 0,
	BLUE,
	GREEN,
	CYAN,
	RED,
	MAGENTA,
	BROWN,
	LIGHT_GRAY,
	DARK_GRAY,
	LIGHT_BLUE,
	LIGHT_GREEN,
	LIGHT_CYAN,
	LIGHT_RED,
	LIGHT_MAGENTA,
	LIGHT_BROWN,
	WHITE
} VGAColor;

void ncPrint(const char * string);
void ncPrintChar(char character);
void ncNewline();
void ncPrintDec(uint64_t value);
void ncPrintHex(uint64_t value);
void ncPrintBin(uint64_t value);
void ncPrintBase(uint64_t value, uint32_t base);
void ncClear();
void ncPrintColor(const char *string, VGAColor foreground, VGAColor background);
void ncPrintCharColor(const char character,uint8_t color);
void ncBackspace();
void TimeClock();
void scroll();
#endif