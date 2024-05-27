#include <stdint.h>
#include <naiveConsole.h>
#include <systemCalls.h>
#include <keyboard.h>
#include <time.h>


int sys_write(uint64_t fd, char * buffer, uint64_t size) {
    if (buffer == 0 || size == 0 || fd > 2){
        return -1;
    }
    uint64_t i = 0;
    while(i < size && buffer[i]){
      ncPrint(buffer[i++]);
    }
    return i;
}

// Note: r10 & r8 are used for screen id and syscall id respectively.
int sysCallDispatcher(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t r10, uint64_t r8) {
  switch(r8){
      case 1:
        return sys_write(rdi, (char *)rsi, rdx);
  }
  return -1;
}