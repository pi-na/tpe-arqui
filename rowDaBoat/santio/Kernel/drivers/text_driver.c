#include <text_driver.h>
#include <stdint.h>

static char buff[MAX_BUFFER_LEN] = {0};
static int buff_size = 0;



char * getBuffer(){
    return buff;
}

char getChar(){
    char c = getLastFromBuffer();
    return c;
}

void putInBuffer(char c){
    if(buff_size == MAX_BUFFER_LEN ){
        buff_size = 0;
    }
    buff[buff_size] = c;
    buff_size ++;
}

void removeLastFromBuffer(){
    buff_size--;
    if(buff_size<=0){
        buff_size = 0;
    }
}

void resetBuffer(){
    buff_size = 0;
}

int getBuffSize(){
    return buff_size;
}

char getLastFromBuffer(){
    if(buff_size <=0){
        return 0;
    }
    return buff[--buff_size];
}