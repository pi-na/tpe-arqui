#ifndef _VIDEODRIVER_H_
#define _VIDEODRIVER_H_

#include <stdint.h>

typedef struct term_color{
    uint8_t RED;
    uint8_t GREEN;
    uint8_t BLUE;
}term_color;


void printChar(uint8_t c);

void printCharFormat(uint8_t c, term_color * charColor, term_color * bgColor);

void print(const char * string);

void printDec(uint64_t value);




void startScreen();

void restartTBlock();

void setScreen(uint8_t screen_id);



void printHex(uint64_t value);


struct vbe_mode_info_structure {
	uint16_t attributes;			// deprecated, only bit 7 should be of interest to you, and it indicates the mode supports a linear frame buffer.
	uint8_t screen_a;				// deprecated
	uint8_t screen_b;				// deprecated
	uint16_t granularity;			// deprecated; used while calculating bank numbers
	uint16_t screen_size;
	uint16_t segment_a;
	uint16_t segment_b;
	uint32_t win_func_ptr;			// deprecated; used to switch banks from protected mode without returning to real mode
	uint16_t pitch;					// number of bytes per horizontal line
	uint16_t width;					// width in pixels
	uint16_t height;				// height in pixels
	uint8_t w_char;					// unused...
	uint8_t y_char;					// ...
	uint8_t planes;
	uint8_t bpp;					// bits per pixel in this mode
	uint8_t banks;					// deprecated; total number of banks in this mode
	uint8_t memory_model;
	uint8_t bank_size;				// deprecated; size of a bank, almost always 64 KB but may be 16 KB...
	uint8_t image_pages;
	uint8_t reserved0;

	uint8_t red_mask;
	uint8_t red_position;
	uint8_t green_mask;
	uint8_t green_position;
	uint8_t blue_mask;
	uint8_t blue_position;
	uint8_t reserved_mask;
	uint8_t reserved_position;
	uint8_t direct_color_attributes;

	uint32_t framebuffer;			// physical address of the linear frame buffer; write here to draw to the screen
	uint32_t off_screen_mem_off;
	uint16_t off_screen_mem_size;	// size of memory in the framebuffer but not being displayed on the screen
	uint8_t reserved1[206];
} __attribute__ ((packed));

typedef struct screen{
    uint8_t actual_i; 				// current pixel
	uint8_t actual_j; 				// current pixel
    uint8_t first_i;  				// screen first pixel
	uint8_t first_j; 				// screen first pixel
    uint16_t width, height; 		// screen dimensions
} screen;	

static term_color RED = {0xFF,0x00,0x00};
static term_color WHITE = {0xFF,0xFF,0xFF};
static term_color BLACK = {0x00,0x00,0x00};



void printBin(uint64_t value);
void printBase(uint64_t value, uint32_t base);
void printRegisterFormat(uint64_t reg);
uint32_t uintToBase(uint64_t value, char * buffer, uint32_t base);

void clearAll();
void pNewLine();

#endif /* _VIDEODRIVER_H_ */