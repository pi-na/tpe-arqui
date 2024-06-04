#include <naiveConsole.h>
#include <stdlib.h>
#define VBEModeInfoBlock 0x0000000000005C00

static uint32_t uintToBase(uint64_t value, char *buffer, uint32_t base);
static char buffer[64] = {'0'};
static uint8_t *const video = (uint8_t *)0xB8000;
static uint8_t *currentVideo = (uint8_t *)0xB8000;
static const uint32_t width = 80;
static const uint32_t height = 25;
extern int get_seconds();
extern int get_minutes();
extern int get_hours();

void ncPrint(const char *string)
{
	int i;

	for (i = 0; string[i] != 0; i++)
		ncPrintChar(string[i]);
}

uint8_t makeColor(VGAColor foreground, VGAColor background)
{
	return (uint8_t)(foreground | background << 4);
}

void ncPrintColor(const char *string, VGAColor foreground, VGAColor background)
{
	int i;
	for ( i = 0; string[i] != 0; i++)
	{
		ncPrintCharColor(string[i], makeColor(foreground, background));
	}
}

void ncPrintCharColor(const char character, uint8_t color)
{
	*currentVideo = character;
	currentVideo += 1;
	*currentVideo = color;
	currentVideo += 1;
}

void ncPrintChar(char character)
{
	*currentVideo = character;
	currentVideo += 2;
}

void ncBackspace(){
	if(currentVideo>=0xB8002){
		currentVideo -= 2;
		*currentVideo = 0;
	}
}

void ncNewline()
{
	do
	{
		ncPrintChar(' ');
	} while ((uint64_t)(currentVideo - video) % (width * 2) != 0);
}

void ncPrintDec(uint64_t value)
{
	ncPrintBase(value, 10);
}

void ncPrintHex(uint64_t value)
{
	ncPrintBase(value, 16);
}

void ncPrintBin(uint64_t value)
{
	ncPrintBase(value, 2);
}

void ncPrintBase(uint64_t value, uint32_t base)
{
	uintToBase(value, buffer, base);
	ncPrint(buffer);
}

void ncClear()
{
	int i;

	for (i = 0; i < height * width; i++)
		video[i * 2] = ' ';
	currentVideo = video;
}

static uint32_t uintToBase(uint64_t value, char *buffer, uint32_t base)
{
	char *p = buffer;
	char *p1, *p2;
	uint32_t digits = 0;

	// Calculate characters for each digit
	do
	{
		uint32_t remainder = value % base;
		*p++ = (remainder < 10) ? remainder + '0' : remainder + 'A' - 10;
		digits++;
	} while (value /= base);

	// Terminate string in buffer.
	*p = 0;

	// Reverse string in buffer.
	p1 = buffer;
	p2 = p - 1;
	while (p1 < p2)
	{
		char tmp = *p1;
		*p1 = *p2;
		*p2 = tmp;
		p1++;
		p2--;
	}

	return digits;
}

void TimeClock()
{
	ncPrintBase(get_hours()+GMT, 10);
	ncPrintChar(':');
	ncPrintBase(get_minutes(), 10);
	ncPrintChar(':');
	ncPrintBase(get_seconds(), 10);
	return;
}

void scroll(){
	if (currentVideo >= video + 2*width*(height-1)){

		for (int i = 0; i < 2*width*(height-1); i++)
		{
			video[i] = video[i + 2*width];

		}
	}
	currentVideo -= 2*width;
	for (int j = 0; j < width; j++){

		currentVideo[j * 2] = ' ';
		currentVideo[2 * j + 1] = 0x07;
	}

}





