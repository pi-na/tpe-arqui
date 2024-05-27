
#ifndef SYSTEMCALLS_H_
#define SYSTEMCALLS_H_

#include <stdint.h>

#define STDIN  0
#define STDOUT 1
#define STDERR 2

int sys_write(uint64_t fd, char * buffer, uint64_t size);


#endif /* SYSTEMCALLS_H_ */