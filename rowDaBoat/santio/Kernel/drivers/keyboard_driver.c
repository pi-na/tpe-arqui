#include <lib.h>
#include <naiveConsole.h>
#include <videoDriver.h>
#include <text_driver.h>
#include <keyboard_driver.h>
#include <syscalls.h>
#include <stdint.h>


#define PRESS 1
#define RELEASE 2
#define ERROR -1

//#define BUFF_LEN 30
//static int buffSize=0; //cantidad de elementos del buffer
static int ridx=0; //posicion de escritura
static int widx=0; //posicion de lectura

#define LEFT_SHIFT 0x2A
#define RIGHT_SHIFT 0x36
#define CAPS_LOCK 0x3A
#define CTRL 0x1D

//static char buffer[BUFF_LEN]={0};

static int shift = 0;
static int ctrl = 0;
static int capsLock = 0;

static char ScanCodes[256]={250, 250, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=',
'\b', '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n', 0, 'a', 's',
'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0, '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.' };


int getOnAction(int key){
    if(key >= 0x01 && key <= 0x3A) { //teclado
        return PRESS;
    }
    else if (key >= 0x81 && key <= 0xBA) {
        return RELEASE;
    }
    return ERROR;
}

char getCharCode(int key){
    return ScanCodes[key];
}



void keyboard_handler(uint64_t rsp[17]){
    int key = getKey();
    static int capsLockOn = 0;
    int current = getOnAction(key);
    if (current == PRESS){
        if (key == CTRL){
            ctrl = 1;
        }else if (key == LEFT_SHIFT || key == RIGHT_SHIFT){
            shift = 1;
        }else if (key == CAPS_LOCK) { // Caps Lock press
            capsLockOn = 1 - capsLockOn; // Toggle the Caps Lock status
        }
        else {
            if(key == 0x39){ // space
                putInBuffer(' ');
                return;
            }
            if (ctrl && ScanCodes[key] == 'r'){
                updateRegisters(rsp);
                return;
            }
            if (ctrl && ScanCodes[key] == ','){
                setFontSize(-1);
                return;
            }
            if (ctrl && ScanCodes[key] == '.'){
                setFontSize(1);
                return;
            }
            if(ScanCodes[key] == '\b'){
                removeLastFromBuffer();
                putInBuffer('\b');
                return;
            }
            if(ScanCodes[key] == '\t'){
                putInBuffer('\t');
                return;
            }
            if(ScanCodes[key] == '\n'){
                putInBuffer('\n');
                return;
            }
            if( key >= 0 && key <= 256 && ScanCodes[key] != 0 ){
                if (capsLockOn || shift) {
                    putInBuffer(ScanCodes[key]-('a'-'A'));
                }
                else {
                    putInBuffer(ScanCodes[key]);
                }
                return;
            }
        
        }
    } else if (current == RELEASE){
        if (key == LEFT_SHIFT + 0x80 || key == RIGHT_SHIFT + 0x80){
            shift = 0;
        }
        else if (key != CTRL){
            ctrl = 0;
        }
    }
    return;
}
