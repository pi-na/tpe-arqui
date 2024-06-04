#ifndef H_VIDEO_DRIVER
#define H_VIDEO_DRIVER
#include <stdint.h>
#include <colors.h>
void setVideoUserLen(int len);
void printCharAt(char c, int x0, int y0, t_color bgColor, t_color ForeColor);
void putPixel(uint32_t hexColor, uint64_t x, uint64_t y);
void paint(uint32_t color);
void square(uint32_t color, int x, int y, int size);
void write(char c, t_color bgColor, t_color foreColor);
void printNewLine();
void printTab();
void load_video();
void printBackspace();
void printInScreen(char *c, int len);
void font();
void writeStatic(char c);
void cursor();
void setFontSize(int i);
void stopCursor();

#endif