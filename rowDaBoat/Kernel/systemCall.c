#include "videoDriver.h"
#include "keyboard.h"
#include "lib.h"
#include <time.h>
#include "sound.h"

#define STDIN  0
#define STDOUT 1
#define STDERR 2

extern uint8_t hasInforeg;
extern const uint64_t inforeg[17];
extern int _hlt();

extern Color RED;
extern Color WHITE;
extern Color BLACK;

int size = 0;

// #define SYS_CALLS_QTY 14
#define SYS_CALLS_QTY 16

//llena buff con el caracter leido del teclado
static uint64_t sys_read(uint64_t fd, char * buff){
    if (fd != 0){
        return -1;
    }

    *buff = getCharFromKeyboard();
    return 0;
}


static uint64_t sys_write(uint64_t fd, char buffer) {
    if (fd != 1){
        return -1;
    }

    dv_print(buffer,WHITE,BLACK);
    return 1;
}

static uint64_t sys_clear(){
    dv_clear(BLACK);
    return 1;
}

static uint64_t sys_getHours(){
    return getHours();
}

static uint64_t sys_getMinutes(){
    return getMinutes();
}

static uint64_t sys_getSeconds(){
    return getSeconds();
} 

static uint64_t sys_getScrHeight(){
    return dv_getHeight();
}

static uint64_t sys_getScrWidth(){
    return dv_getWidth();
}

static void sys_fillRect (int x, int y, int x2, int y2, Color color){
    dv_fillRect (x,y,x2,y2,color);
}

static void sys_wait (int ms){
    if (ms > 0){
        int start_ms = ms_elapsed();
        do { _hlt(); } while (ms_elapsed() - start_ms < ms);
    }
}

static uint64_t sys_inforeg (uint64_t registers[17]){
    if(hasInforeg){
        for(uint8_t i=0; i<17; i++){
            registers[i] = inforeg[i];
        }
    }
    return hasInforeg;
}

static uint64_t sys_printmem ( uint64_t * address ){
    if((uint64_t) address > (0x20000000 - 32)){
      return -1;
    }

    uint8_t * aux = (uint8_t *) address;
        dv_prints("\n",WHITE,BLACK);
    for(int i=0; i < 32 ; i++){
        dv_printHex((uint64_t) aux, WHITE, BLACK);
        dv_prints(" = ", WHITE, BLACK);
        dv_printHex(*aux, WHITE, BLACK);
        dv_newline();
        aux++;
    }

    return 0;
}


static uint64_t sys_pixelPlus(){
    increasePixelScale();
    sys_clear();
    return 1;
}

static uint64_t sys_pixelMinus(){
    decreasePixelScale();
    sys_clear();
    return 1;
}

static uint64_t sys_playSound(uint32_t frequnce){
    startSound(frequnce);
    return 1;
}

static uint64_t sys_mute(){
    stopSound();
    return 1;
}



// static uint64_t (*syscall_handlers[])(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t r10, uint64_t r8) = {
//     sys_read, sys_write, sys_clear, sys_getHours, sys_getMinutes, sys_getSeconds,sys_getScrHeight, sys_getScrWidth, sys_fillRect,
//     sys_wait, sys_inforeg,sys_printmem, sys_pixelPlus, sys_pixelMinus
// };

//los void los pongo sino me tira warning
static uint64_t (*syscall_handlers[])(uint64_t, uint64_t, uint64_t, uint64_t, uint64_t) = {
    (void*)sys_read, (void*)sys_write, (void*)sys_clear, (void*)sys_getHours, (void*)sys_getMinutes, (void*)sys_getSeconds, (void*)sys_getScrHeight, (void*)sys_getScrWidth, (void*)sys_fillRect,
    (void*)sys_wait, (void*)sys_inforeg, (void*)sys_printmem, (void*)sys_pixelPlus, (void*)sys_pixelMinus, (void*)sys_playSound, (void*)sys_mute
};


// Devuelve la syscall correspondiente
//                                rdi           rsi           rdx           rd10          r8           r9
uint64_t syscall_handler(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t r10, uint64_t r8, uint64_t rax) {
    if (rax < SYS_CALLS_QTY && syscall_handlers[rax] != 0){
        return syscall_handlers[rax](rdi, rsi, rdx, r10, r8);
    }

    return 0;
}