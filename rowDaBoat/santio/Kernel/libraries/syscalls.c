#include <syscalls.h>
#include <stdint.h>
#include <videoDriver.h>
#include <keyboard_driver.h>
#include <text_driver.h>
#include <clock.h>
#include <time.h>
int get_minutes();
int get_hours();
int get_seconds();

#define GMT -3

static uint64_t registers[17] = {0};

void sys_write(char *str, uint8_t len, t_color bgColor, t_color ftColor, int usrLen)
{
	if (str == 0 ||  len <= 0 || bgColor < 0 || ftColor < 0) 
        return ;
	for (int i = 0; str[i] != 0 && i < len; i++) 
	write(str[i], bgColor, ftColor);
}

void sys_clear(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint32_t r9){
	switch (rsi)
	{
	case 0:
		clear();
		break;
	case 1:
		rewrite();
		break;
	default:
		break;
	}
}

void sys_wait(uint64_t rsi, char rdx, uint64_t rcx, uint64_t r8, uint32_t r9){
	sleep(300);
}

uint64_t sys_read(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint32_t r9){
	if(rsi == 5){//read userLen
		setVideoUserLen(rdx);
		return 0;
	}
	if(rsi == 6){
		return getBuffer();
	}
	if(rsi == 7){
		return getBuffSize();
	}
	if(rsi == 8){
		return getChar();
	}

	return 0;
}

uint64_t sys_inforeg(){
	return registers;
}

void updateRegisters(uint64_t rsp[17]){
    for (int i=0 ; i<=16;i++) {
        registers[i] = rsp[i];
    } 
}

static unsigned int decode(unsigned char time){
    return (time >> 4) * 10 + (time & 0x0F); 
}

unsigned int sys_rtc(unsigned char id){
	switch(id){
		case HOURS:
			return (get_hours() + GMT + 24) % 24 ;
		case MINUTES:
			return decode(get_minutes());
		case SECONDS:
			return decode(get_seconds());
		default:
			break;
	}
}


void sys_drawSquare(int size, int x, int y, int color){
	drawSquare(size, x, y, color);
}