#include "eliminator.h"
#include <userlib.h>
#include <colors.h>
#include "sys_calls.h"

static unsigned long int next = 1;

#define WIDTH 40
#define HEIGHT 40
#define MAXDIM 100

#define PIXELWIDTH (get_scrWidth() / WIDTH)
#define PIXELHEIGHT (get_scrHeight() / HEIGHT)
#define MAX_BUFF 254

#define KEY_UP_P1 'w'
#define KEY_DOWN_P1 's'
#define KEY_LEFT_P1 'a'
#define KEY_RIGHT_P1 'd'

#define KEY_UP_P2 'i'
#define KEY_DOWN_P2 'k'
#define KEY_LEFT_P2 'j'
#define KEY_RIGHT_P2 'l'

typedef struct Player {
    int alive;
    int currentX;
    int currentY;
    Color playerColor;
    int facingDirection;
    int position[2];
    char symbol;
} PlayerType;


int randint() {
    next = next * 1103515245 + 12345;
    return (unsigned int)(next / 65536) % 32768;
}

//0 is empty, 1 is player1 trail, 2 is player2 trail
int gameStatus[WIDTH][HEIGHT] = {0}; 

void draw(PlayerType* player1, PlayerType* player2) {
    int i, j;
    for(i = 0; i<WIDTH; i++){
        for(j=0; j<HEIGHT; i++){
            switch(gameStatus[i][j]){
                case 1:
                    sys_fillRect(i*PIXELWIDTH, j*PIXELHEIGHT, PIXELWIDTH, PIXELHEIGHT, player1->playerColor);
                    break;
                case 2:
                    sys_fillRect(i*PIXELWIDTH, j*PIXELHEIGHT, PIXELWIDTH, PIXELHEIGHT, player2->playerColor);
                    break;

            }
        }
    }
}

void drawBoard2(char game[HEIGHT][WIDTH], struct Player *player1, struct Player *player2) {
    Color currentColor;
    int i, j;
    for (i = 0; i < HEIGHT; i++) {
        for (j = 0; j < WIDTH; j++) {
            if (game[i][j] == ' ') {
                currentColor = WHITE;
            } else if (game[i][j] == player1->symbol) {
                currentColor = player1->playerColor;
            } else if (game[i][j] == player2->symbol) {
                currentColor = player2->playerColor;
            } else if (game[i][j] == '*') {
                currentColor = RED;
            }
            fill_rect(j * PIXELWIDTH, i * PIXELHEIGHT, PIXELWIDTH - 1, PIXELHEIGHT - 1, currentColor);
        }
    }
}
