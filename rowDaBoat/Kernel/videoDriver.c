#include <videoDriver.h>
#include <fonts.h>
 

static char buffer[64] = { '0' };
static const struct vbe_mode_info_structure * graphicModeInfo = (struct vbe_mode_info_structure *) 0x5C00;

static void nextPos();
static void checkSpace();
static void scrollUp();

uint8_t as_id = 0;             
term_color * font_color = &WHITE;  
term_color * bg_color = &BLACK;   

// as = actual screen
screen as[4];


static uint8_t * getPixelAddress(int i, int j) {
    return (uint8_t *) (graphicModeInfo->framebuffer+3*(graphicModeInfo->width*i+j));
}

static void drawPixel(int i, int j, term_color * color){
    uint8_t * pixel = getPixelAddress(i,j);
    pixel[0] = color->BLUE;
    pixel[1] = color->GREEN;
    pixel[2] = color->RED;
  }

void startScreen(){
    as_id = 0;
    as[0].actual_i = 0;
    as[0].actual_j = 0;
    as[0].first_i = 0;
    as[0].first_j = 0;
    as[0].width = graphicModeInfo->width / CHAR_WIDTH;
    as[0].height = graphicModeInfo->height / CHAR_HEIGHT;
    clearAll();
}

void printCharFormat(uint8_t c, term_color * charColor, term_color * bgColor){
   
    if(c == '\b'){
      if(as[as_id].actual_j == 0){        
          as[as_id].actual_i -= 1;                               
          as[as_id].actual_j = as[as_id].width-1;
          printCharFormat(' ', charColor, bgColor);
          as[as_id].actual_i -= 1;
          as[as_id].actual_j = as[as_id].width-1;  
      } else {
        as[as_id].actual_j = (as[as_id].actual_j-1) % as[as_id].width;
        printCharFormat(' ', charColor, bgColor);
        as[as_id].actual_j = (as[as_id].actual_j-1) % as[as_id].width;  
      }
      return;
  }
  
    checkSpace();

    if(c == '\n'){
        pNewLine();
        return;
    }

    uint8_t * character = getCharMapping(c);
    uint16_t write_i = (as[as_id].first_i + as[as_id].actual_i) * CHAR_HEIGHT;
    uint16_t write_j = (as[as_id].first_j + as[as_id].actual_j) * CHAR_WIDTH;

    uint8_t mask;

    for(int i=0; i < CHAR_HEIGHT; ++i){
        for(int j=0; j < CHAR_WIDTH; ++j){
            mask = 1 << (CHAR_WIDTH - j - 1);
            if(character[i] & mask){
                drawPixel(write_i + i, write_j + j, charColor);
            }
            else{
                drawPixel(write_i + i, write_j + j, bgColor);
            }
        }
    }
    nextPos();
}

static void nextPos(){
    as[as_id].actual_i += ((as[as_id].actual_j + 1) == as[as_id].width ) ? 1:0;
    as[as_id].actual_j = (as[as_id].actual_j + 1) % as[as_id].width;
}

static void checkSpace(){
    if(as[as_id].actual_i == as[as_id].height){
        scrollUp();
    }
}

static void scrollUp(){
    for(int i=1; i < as[as_id].height * CHAR_HEIGHT; ++i){

        uint8_t * start = getPixelAddress(as[as_id].first_i + i, as[as_id].first_j);
        uint8_t * next = getPixelAddress(as[as_id].first_i + CHAR_HEIGHT + i, as[as_id].first_j);

        for(int j=0; j < as[as_id].width * CHAR_WIDTH * 3 ; ++j){
            start[j] = next[j];
        }
    }
    as[as_id].actual_i -= 1;
}

void printChar(uint8_t c){
    printCharFormat(c, &WHITE, &BLACK);
}

void setScreen(uint8_t screen_id){
    as_id = screen_id;
}

void print(const char * string){
    for (int i=0; string[i] != 0; ++i){
       printChar(string[i]);
    }
}

void pNewLine(){
    as[as_id].actual_j = 0;
    as[as_id].actual_i += 1;
}

void restartTBlock(){
    as[as_id].actual_i = 0;
    as[as_id].actual_j = 0;
}

void clearAll(){
    as[as_id].actual_i = 0;
    as[as_id].actual_j = 0;
    for(int i=0; i < as[as_id].height ; ++i ){
        for(int j=0; j < as[as_id].width ; ++j){
            printCharFormat(' ', &WHITE, &BLACK);
        }
    }
    as[as_id].actual_i = 0;
    as[as_id].actual_j = 0;
}


void printDec(uint64_t value){
	  printBase(value, 10);
}

void printHex(uint64_t value){
	  printBase(value, 16);
}

void printBin(uint64_t value){
	  printBase(value, 2);
}

void printBase(uint64_t value, uint32_t base){
    uintToBase(value, buffer, base);
    print(buffer);
}

uint32_t uintToBase(uint64_t value, char * buffer, uint32_t base){
    char *p = buffer;
    char *p1;
    char *p2;
    uint32_t digits = 0;

    //Calculate characters for each digit
    do{
        uint32_t remainder = value % base;
        *p++ = (remainder < 10) ? remainder + '0' : remainder + 'A' - 10;
        digits++;
    }
    while (value /= base);

    // Terminate string in buffer.
    *p = 0;
    //Reverse string in buffer.
    p1 = buffer;
    p2 = p - 1;
    while (p1 < p2){
        char tmp = *p1;
        *p1 = *p2;
        *p2 = tmp;
        p1++;
        p2--;
    }
    return digits;
}


void printRegisterFormat(uint64_t reg){
	
    uint64_t aux = reg;
    uint64_t count =  16;
    
    while(aux){
        aux = aux >> 4;
        --count;
    }

    for(int i=0; i < count ;i++){
       printChar('0');
    }
  
    if(reg){
       printHex(reg);
    }
}