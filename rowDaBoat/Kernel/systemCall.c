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
#define SYS_CALLS_QTY 19

//llena buff con el caracter leido del teclado
static uint64_t sys_read(uint64_t fd, char * buff){
    if (fd != 0){
        return -1;
    }

    *buff = getCharFromKeyboard();
    return 0;
}

static int sys_drawCursor(){
    vDriver_drawCursor();
    return 1;
}

static uint64_t sys_write(uint64_t fd, char buffer) {
    if (fd != 1){
        return -1;
    }

    vDriver_print(buffer,WHITE,BLACK);
    return 1;
}

static uint64_t sys_writeColor(uint64_t fd, char buffer, Color color) {
    if (fd != 1){
        return -1;
    }

    vDriver_print(buffer, color, BLACK);
    return 1;
}

static uint64_t sys_clear(){
    vDriver_clear(BLACK);
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
    return vDriver_getHeight();
}

static uint64_t sys_getScrWidth(){
    return vDriver_getWidth();
}

static void sys_drawRectangle (int x, int y, int x2, int y2, Color color){
    vDriver_drawSquare (x,y,x2,y2,color);
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
        vDriver_prints("\n",WHITE,BLACK);
    for(int i=0; i < 32 ; i++){
        vDriver_printHex((uint64_t) aux, WHITE, BLACK);
        vDriver_prints(" = ", WHITE, BLACK);
        vDriver_printHex(*aux, WHITE, BLACK);
        vDriver_newline();
        aux++;
    }

    return 0;
}


static uint64_t sys_pixelPlus(){
    plusScale();
    return 1;
}

static uint64_t sys_pixelMinus(){
    minusScale();
    sys_clear();
    return 1;
}

static uint64_t sys_playBeep(uint32_t frequence, uint64_t duration){
    beep(frequence, duration);
    return 1;
}

static uint64_t sys_mute(){
    mute();
    return 1;
}

// los void los pongo sino me tira warning
static uint64_t (*syscalls[])(uint64_t, uint64_t, uint64_t, uint64_t, uint64_t) = {
                                    // RAX: syscall number
    (void*)sys_read,                // 0
    (void*)sys_write,               // 1
    (void*)sys_clear,               // 2
    (void*)sys_getHours,            // 3
    (void*)sys_getMinutes,          // 4
    (void*)sys_getSeconds,          // 5
    (void*)sys_getScrHeight,        // 6
    (void*)sys_getScrWidth,         // 7
    (void*)sys_drawRectangle,       // 8
    (void*)sys_wait,                // 9
    (void*)sys_inforeg,             // 10
    (void*)sys_printmem,            // 11
    (void*)sys_pixelPlus,           // 12
    (void*)sys_pixelMinus,          // 13
    (void*)sys_playBeep,            // 14
    (void*)sys_mute,                // 15
    (void*)sys_drawCursor,          // 16
    (void*)sys_writeColor           // 17
};

// Devuelve la syscall correspondiente
//                              rdi           rsi           rdx           rd10          r8           r9
uint64_t syscall_dispatcher(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t r10, uint64_t r8, uint64_t rax) {
    if (rax < SYS_CALLS_QTY && syscalls[rax] != 0){
        return syscalls[rax](rdi, rsi, rdx, r10, r8);
    }

    return 0;
}