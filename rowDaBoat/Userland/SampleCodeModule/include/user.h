#ifndef _USER_H_
#define _USER_H_

#include <stdint.h>

#define STDIN 0
#define STDOUT 1
#define STDERR 2

int sprint(uint8_t fd, char * str);

#endif /* _USER_H_ */