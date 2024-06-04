#ifndef TEXT_DRIVER_H
#define TEXT_DRIVER_H

#define MAX_BUFFER_LEN 30

int getBuffSize();

char * getBuffer();

void resetBuffer();

void removeLastFromBuffer();

void putInBuffer(char c);

char getLastFromBuffer();

#endif