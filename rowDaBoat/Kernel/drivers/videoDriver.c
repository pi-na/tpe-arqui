#include <videoDriver.h>
#include <font.h>
#include <time.h>

Color WHITE = {255,255,255};
Color BLACK = {0,0,0};

uint8_t pixelScale = 1;
const uint16_t WIDTH_FONT = 9;
const uint16_t HEIGHT_FONT = 16;

uint16_t cursorOn = 0;
uint16_t cursorX = 0;
uint16_t cursorY = 0;
static char buffer[64] = { '0' };

static void drawChar(int x, int y, unsigned char c,Color frontColorColor, Color bgColor);
static void scroll();
static uint32_t uintToBase(uint64_t value, char * buffer, uint32_t base);
static uint32_t* getPixelPos(uint16_t x, uint16_t y);

struct vbe_mode_info_structure {
    uint16_t attributes;
    uint8_t window_a;
    uint8_t window_b;
    uint16_t granularity;
    uint16_t window_size;
    uint16_t segment_a;
    uint16_t segment_b;
    uint32_t win_func_ptr;
    uint16_t pitch;
    uint16_t width;
    uint16_t height;
    uint8_t w_char;
    uint8_t y_char;
    uint8_t planes;
    uint8_t bpp;                
    uint8_t banks;
    uint8_t memory_model;   
    uint8_t bank_size;          
    uint8_t image_pages;
    uint8_t reserved0;

    uint8_t red_mask;
    uint8_t red_position;
    uint8_t green_mask;
    uint8_t green_position;
    uint8_t blue_mask;
    uint8_t blue_position;
    uint8_t reserved_mask;
    uint8_t reserved_position;
    uint8_t direct_color_attributes;

    uint32_t framebuffer;           
    uint32_t off_screen_mem_off;
    uint16_t off_screen_mem_size;   
    uint8_t reserved1[206];
} __attribute__ ((packed));         
struct vbe_mode_info_structure* vbeInfo = (void*)0x5C00;

// Static methods
static uint32_t* getPixelPos(uint16_t x, uint16_t y) {
    uint8_t width = vbeInfo->bpp/8;  
    uint16_t height = vbeInfo->pitch;

    uintptr_t pos = (uintptr_t)(vbeInfo->framebuffer) + (x * width) + (y * height);
    return (uint32_t*)pos;
}

static void drawChar(int xPos, int yPos, unsigned char character, Color fgColor, Color bgColor) {
    int xOffset, yOffset;
    int bitMask[8] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80};
    const unsigned char *glyphData = font_bitmap + 16 * (character - 32);

    if (cursorX >= vbeInfo->width) {
        cursorX = 0;
        if (cursorY + (HEIGHT_FONT * pixelScale) > vbeInfo->height) {
            cursorY -= (HEIGHT_FONT * pixelScale);
            scroll();
        } else {
            cursorY += (HEIGHT_FONT * pixelScale);
        }
    }

    for (yOffset = 0; yOffset < 16; yOffset++) {
        for (xOffset = 0; xOffset < 8; xOffset++) {
            for (int i = 0; i < pixelScale; i++) {
                for (int j = 0; j < pixelScale; j++) {
                    if (glyphData[yOffset] & bitMask[xOffset]) {
                        video_setPixel(cursorX + (8 - xOffset) * pixelScale + i, cursorY + yOffset * pixelScale + j, fgColor);
                    } else {
                        video_setPixel(cursorX + (8 - xOffset) * pixelScale + i, cursorY + yOffset * pixelScale + j, bgColor);
                    }
                }
            }
        }
    }

    cursorX += WIDTH_FONT * pixelScale;
}

static void scroll(){
    Color* pixel, *next;
    int yLimit = cursorY + HEIGHT_FONT*pixelScale;
    for (int i = 0 ; i < yLimit; i++){
        for (int j = 0 ; j < vbeInfo->width ; j++){
            pixel = (Color *) getPixelPos(j,i);
            next = (Color *) getPixelPos(j,i+HEIGHT_FONT*pixelScale);
            *pixel = *next;
        }
    }
}

// Función auxiliar para invertir una cadena
static void reverse(char *str, uint32_t length) {
    uint32_t start = 0;
    uint32_t end = length - 1;
    while (start < end) {
        char temp = str[start];
        str[start] = str[end];
        str[end] = temp;
        start++;
        end--;
    }
}

static uint32_t uintToBase(uint64_t value, char *buffer, uint32_t base) {
    if (base < 2 || base > 36) {
        *buffer = '\0';
        return 0; // base inválida
    }

    uint32_t index = 0;
    do {
        uint32_t remainder = value % base;
        buffer[index++] = (remainder < 10) ? (remainder + '0') : (remainder + 'A' - 10);
        value /= base;
    } while (value > 0);

    buffer[index] = '\0'; // Termina la cadena
    reverse(buffer, index); // Invierte la cadena para obtener el resultado correcto
    return index;
}

// Methods used for sysCalls
uint16_t getWidth(void) {
    return vbeInfo->width;
}
uint16_t getHeight(void) {
    return vbeInfo->height;
}

void plusScale() {
    if (pixelScale < 5) pixelScale++;
}

void minusScale() {
    if (pixelScale > 1) pixelScale--;
}


void video_drawCursor() {
    int xPos, yPos;
    Color fgColor = cursorOn ? BLACK : WHITE;
    Color bgColor = cursorOn ? BLACK : WHITE;

    int bitMask[8] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80};
    const unsigned char *glyphData = font_bitmap + 16 * (' ' - 32);

    if (cursorX >= vbeInfo->width) {
        cursorX = 0;
        if (cursorY + (HEIGHT_FONT * pixelScale) > vbeInfo->height) {
            cursorY -= (HEIGHT_FONT * pixelScale);
            scroll();
        } else {
            cursorY += (HEIGHT_FONT * pixelScale);
        }
    }

    for (yPos = 0; yPos < 16; yPos++) {
        for (xPos = 0; xPos < 8; xPos++) {
            for (int i = 0; i < pixelScale; i++) {
                for (int j = 0; j < pixelScale; j++) {
                    if (glyphData[yPos] & bitMask[xPos]) {
                        video_setPixel(cursorX + (8 - xPos) * pixelScale + i, cursorY + yPos * pixelScale + j, fgColor);
                    } else {
                        video_setPixel(cursorX + (8 - xPos) * pixelScale + i, cursorY + yPos * pixelScale + j, bgColor);
                    }
                }
            }
        }
    }

    cursorOn = !cursorOn;
}

void video_clear() {
    Color color = BLACK;
    Color* pixel = (Color*) ((uint64_t)vbeInfo->framebuffer);

    for (uint32_t len = (uint32_t)vbeInfo->width * vbeInfo->height; len; len--, pixel++){
        *pixel = color;
    }
    cursorX = 0;
    cursorY = 0;
}

void video_printDec(uint64_t value, Color frontColor, Color backgroundColor){
    video_printBase(value, 10,frontColor,backgroundColor);
}

void video_printHex(uint64_t value, Color frontColor, Color backgroundColor){
    video_printBase(value, 16,frontColor,backgroundColor);
}

void video_printBin(uint64_t value, Color frontColor, Color backgroundColor){
    video_printBase(value, 2,frontColor,backgroundColor);
}

void video_printBase(uint64_t value, uint32_t base, Color frontColor, Color backgroundColor){
    uintToBase(value, buffer, base);
    for (int i = 0 ; buffer[i] != '\0' ; i++ ){
        video_print(buffer[i], frontColor, backgroundColor);
    }
}
 
void video_setPixel(uint16_t x, uint16_t y, Color color) {
    if (x >= vbeInfo->width || y >= vbeInfo->height)
        return;

    Color* pixel = (Color*) getPixelPos(x, y);
    *pixel = color;
}

void video_drawRectangle(int x, int y, int w, int h, Color color){
    Color * pixel;

    for (int i = 0 ; i < h ; i++){
        pixel = (Color*) getPixelPos(x,y+i);
        for (int j = 0 ; j < w ; j++, pixel++){
            *pixel = color;
        }
    }
}

void video_prints(const char *str, Color frontColor, Color backgroundColor){
    for (int i = 0 ; str[i] != '\0'; i++ ){
        video_print(str[i], frontColor, backgroundColor);
    }
}

void video_print(const char c, Color frontColor, Color backgroundColor){
    switch (c) {
        case '\n':
            video_newline();
            break;
        case '\b':
            video_backspace(frontColor, backgroundColor);
            break;
        case '\0':
            break;  //
        default:
            drawChar(cursorX, cursorY ,c ,frontColor ,backgroundColor);
        break;
    }
}

void video_newline(){
    cursorX = 0;
    cursorY += HEIGHT_FONT* pixelScale;

    if (cursorY + HEIGHT_FONT*pixelScale > vbeInfo->height){
        cursorY -= HEIGHT_FONT*pixelScale;
        scroll();
    }
}

void video_backspace(Color frontColor, Color backgroundColor){
    if (cursorX >= WIDTH_FONT*pixelScale){
        cursorX -= WIDTH_FONT*pixelScale;
    } else {
        cursorX = 0;
    }
    drawChar(cursorX, cursorY , ' ' , frontColor , backgroundColor);
    cursorX -= WIDTH_FONT*pixelScale;
}
