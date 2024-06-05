#ifndef _VIDEO_DRIVER_H_
#define _VIDEO_DRIVER_H_

#include <stdint.h>

typedef struct {
    uint8_t b;
    uint8_t g;
    uint8_t r;
} Color;
extern Color WHITE;
extern Color BLACK;

void plusScale();
void minusScale();

void video_prints(const char *str, Color fnt, Color bgd);
void video_print(char c, Color fnt, Color bgd);
void video_newline();
void video_backspace(Color fnt, Color bgd);
void video_clear();

void video_printDec(uint64_t value, Color fnt, Color bgd);
void video_printHex(uint64_t value, Color fnt, Color bgd);
void video_printBin(uint64_t value, Color fnt, Color bgd);
void video_printBase(uint64_t value, uint32_t base, Color fnt, Color bgd);

void video_drawCursor();
void video_drawRectangle(int x, int y, int x2, int y2, Color color);
void video_setPixel(uint16_t x, uint16_t y, Color color);

/* Getters for screen */
uint16_t getWidth(void);
uint16_t getHeight(void);


#endif