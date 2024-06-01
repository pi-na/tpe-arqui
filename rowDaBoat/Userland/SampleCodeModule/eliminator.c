#include "eliminator.h"
#include "sys_calls.h"
#include <userlib.h>
#include <colors.h>

#define WIDTH 60
#define HEIGHT 60
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
    int currentX;
    int currentY;
    Color playerColor;
    int facingDirection;
    int position[2];
} PlayerType;

void draw(PlayerType* players, int playerCount) {
    for(int i = 0; i<playerCount; i++){
        drawRectangle(players[i].currentX*PIXELWIDTH, players[i].currentY*PIXELHEIGHT, PIXELWIDTH, PIXELHEIGHT, players[i].playerColor);
    }
}

void checkInput(PlayerType player1, PlayerType player2) {
    switch (getChar()) {
        case KEY_UP_P1:
            player1.facingDirection = 0;
            break;
        case KEY_DOWN_P1:
            player1.facingDirection = 1;
            break;
        case KEY_LEFT_P1:
            player1.facingDirection = 2;
            break;
        case KEY_RIGHT_P1:
            player1.facingDirection = 3;
            break;
        case KEY_UP_P2:
            player2.facingDirection = 0;
            break;
        case KEY_DOWN_P2:
            player2.facingDirection = 1;
            break;
        case KEY_LEFT_P2:
            player2.facingDirection = 2;
            break;
        case KEY_RIGHT_P2:
            player2.facingDirection = 3;
            break;
        default:
            break;
    }
}

// Returns 0 if no collisions, returns 1 if player1 collides with an existing trail in the game matrix,
// returns 2 if player2 collides with an existing trail in the game matrix
int checkCollision(PlayerType* players, int playerCount, int** gameStatus){
    for(int i = 0; i<playerCount; i++){
        if(gameStatus[players[i].currentY][players[i].currentX] != 0){
            return i+1;
        }
    }
    return 0;
}

void eliminator(int playerCount){
    PlayerType player1 = {WIDTH/3, HEIGHT/3, RED, 3, {WIDTH/3, HEIGHT/3}};
    PlayerType player2 = {WIDTH/2, HEIGHT/2, BLUE, 0, {WIDTH/2, HEIGHT/2}};
    PlayerType players[2] = {player1, player2};

    //0 is empty, 1 is player1 trail, 2 is player2 trail
    //TODO, podemos guardar solo - 0 o 1
    prints("Eliminator game started\n", MAX_BUFF);
    int gameStatus[HEIGHT][WIDTH] = {0};
    int loser = 0;

    while(!loser){
        draw(players, playerCount);
        checkInput(players[0], players[1]);
        updatePlayerPosition(players, playerCount, gameStatus);
        //loser = checkCollision(players, playerCount, gameStatus);
        wait(100);
    }
    gameOverScreen(loser);
    return;
}

void gameOverScreen(int loser){
    drawRectangle(0, 0, get_scrWidth() / 2, get_scrHeight() / 8, BLACK);
    prints("\nGame Over. Player ", MAX_BUFF);
    printc(loser + '0');
    prints("LOST!\nPress 'space' to play again or 'q' to quit", MAX_BUFF);
    char c;
    while ( (c = getChar() != 'q') && (c != ' ')){
        continue;
    }
    if(c == 'r'){
        eliminator(2);
    }
    clear_scr();
    return;
}

void updatePlayerPosition(PlayerType* players, int playerCount, int** gameStatus){
    for (int i=0; i<playerCount; i++){
        switch (players[i].facingDirection) {
            case 0: // Up
                players[i].currentY--;
                gameStatus[players[i].currentY][players[i].currentX] = 1;
                break;
            case 1: // Down
                players[i].currentY++;
                gameStatus[players[i].currentY][players[i].currentX] = 1;
                break;
            case 2: // Left
                players[i].currentX--;
                gameStatus[players[i].currentY][players[i].currentX] = 1;
                break;
            case 3: // Right
                players[i].currentX++;
                gameStatus[players[i].currentY][players[i].currentX] = 1;
                break;
            default:
                break;
        }
    }
}
