// keyboard.h
#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <stdint.h>

extern unsigned int keyRead();
unsigned int getScanCode();
unsigned char ctoi(unsigned char mChar);
char *  tryKeyboard();
void keyboard_handler();

#endif