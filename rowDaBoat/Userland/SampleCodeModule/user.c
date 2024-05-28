#include <user.h>


extern int  sys_write(uint64_t fd, char * buffer, uint64_t size);

int sprint(uint8_t fd, char * str){
  return sys_write(fd, str, _strlen(str));
}

int _strlen(const char * str){
    int i=0;
    while( *(str + (i++)) );
    return i-1;
}