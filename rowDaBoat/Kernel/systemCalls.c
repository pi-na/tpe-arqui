#include <systemCalls.h>
#include <keyboard.h>
#include <time.h>

#include <stdint.h>
#include <naiveConsole.h>
#include <videoDriver.h>

int sys_hasTicked(){
    static unsigned long last_tick = 0;
    unsigned long current_tick = ticks_elapsed();
    if(last_tick == current_tick){
        return 0;
    }
    last_tick = current_tick;
    return 1;
}

void sys_clearWindow(){
    clearAll();
}

void sys_restartTBlock(){
    restartTBlock();
}

void sys_setScreen(uint8_t id){
    setScreen(id);
}


int sys_write(uint64_t fd, char * buffer, uint64_t size) {
    if (buffer == 0 || size == 0 || fd > 2){
        return -1;
    }

    term_color col = RED;
    uint64_t i = 0;
    while(i < size && buffer[i]){
      printCharFormat(buffer[i++],&col, &BLACK);
    }
    return i;
}
void sys_startScreen(){
    startScreen();
}

int sys_printmem(uint64_t * mem_address){
    if((uint64_t) mem_address > (0x20000000 - 32)){
      return -1;
    }

    uint8_t * aux = (uint8_t *) mem_address;
    for(int i=0; i < 32 ; ++i){
        printHex((uint64_t) aux);
        print(" = ");
        printHex(*aux);
        pNewLine();
        ++aux;
    }
    return 0;
}



int sys_read(uint64_t fd, char * buffer, uint64_t size) {
    if (buffer == 0 || size == 0 || fd != 0){
      return -1;
    }
    uint8_t i = 0;
    int c;

    while(i<size && ((c = getChar()) != -1)){
      buffer[i++] = c;
    }
    return i;
}


static char store[900];
void store_registers(uint64_t * start){

  char * reg_text[] = {"RAX: 0x", "R15: 0x", "R14: 0x", "R13: 0x", "R12: 0x", "R11: 0x", "R10: 0x", "R9:  0x",
                       "R8:  0x", "RSI: 0x", "RDI: 0x", "RBP: 0x", "RDX: 0x", "RCX: 0x", "RBX: 0x", "RSP: 0x", 0};
  uint32_t j = 0; //store counter

  for(int i=0 ; reg_text[i] ; ++i){
    //Agregamos el string al store
    int m = 0;
    while(reg_text[i][m]){
      store[j++] = reg_text[i][m++];
    }

    //Agregamos el nro al store
    uint64_t aux = start[i];
    int64_t count =  16;
    while(aux){
        aux = aux >> 4;
        --count;
    }
    for(int k=0; k < count ;k++){
       store[j++] = '0';
    }

    if(start[i]){
      j += uintToBase(start[i], store+j, 16);
    }
    store[j++] = '\n';
  }
  store[j] = 0;
}

void sys_infoReg(){
  if(!store[0]){
    print("Debes presionar '-' para guardar el estado de los registros en un momento especifico \n");
  }
  print(store);
}

int sysCallDispatcher(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t r10, uint64_t r8) {
  switch(r8){
      case 0:
        return sys_read(rdi, (char *)rsi, rdx);

      case 1:
        return sys_write(rdi, (char *)rsi, rdx);

      case 3:
        return sys_hasTicked();

      case 4:
        sys_clearWindow();
        return 0;

      case 5:
        sys_restartTBlock();
        return 0;

      case 7:
        sys_startScreen();
        return 0;

      case 8:
        return sys_printmem((uint64_t *) rdi);

      case 9:
        sys_setScreen(rdi);
        return 0;


      case 11:
        sys_infoReg();
        return 0;
  }
  return -1;
}