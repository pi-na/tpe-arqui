
#ifndef SYSTEMCALLS_H_
#define SYSTEMCALLS_H_

#include <stdint.h>

#define STDIN  0
#define STDOUT 1
#define STDERR 2


void sys_startScreen();
int sys_printmem(uint64_t * mem_address);
void sys_setScreen(uint8_t id);
void store_registers(uint64_t * start);
void sys_infoReg();

int sys_write(uint64_t fd, char * buffer, uint64_t size);
int sys_read(uint64_t fd, char * buffer, uint64_t size);
int sys_hasTicked();
void sys_clearWindow();
void sys_restartTBlock();
void store_registers(uint64_t * start);


#endif /* SYSTEMCALLS_H_ */