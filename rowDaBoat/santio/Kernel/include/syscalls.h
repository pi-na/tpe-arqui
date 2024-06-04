#ifndef SYSCALLS_H
#define SYSCALLS_H
#include <stdint.h>
#include <colors.h>

#define SYS_RTC_ID 1
#define SYS_CPUFEATURES_ID 2
#define SYS_READ_ID 3
#define SYS_WRITE_ID 4
#define SYS_INFOREG_ID 5
#define SYS_PRINTMEM_ID 6
#define SYS_CPUID_ID 7
#define SYS_CLEAR_ID 8
#define SYS_EXIT_ID 9
#define SYS_CPUVENDOR_ID 10
#define SYS_WAIT_ID 11
#define SYS_DRAW_ID 12
#define SYS_TICKS_ID 13
#define SYS_SOUND_ID 14
#define SYS_MUTE_ID 15

//
void sys_write(char * str, uint8_t len, t_color bgColor, t_color ftColor, int usrLen);
uint64_t sys_read(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint32_t r9);
void sys_wait(uint64_t rsi, char rdx, uint64_t rcx, uint64_t r8, uint32_t r9);
void sys_clear(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint32_t r9);
uint64_t sys_inforeg();
void updateRegisters(uint64_t rsp[17]);
unsigned int sys_rtc(unsigned char rsi);
void sys_drawSquare(int size, int x, int y, int color);

#endif