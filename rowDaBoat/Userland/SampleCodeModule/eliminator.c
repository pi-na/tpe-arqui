#include "eliminator.h"
#include "sys_calls.h"
#include <userlib.h>
#include <colors.h>

//TODO SACAR POSITION
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

void checkInput(PlayerType* players) {
    char input = getChar();

    int p = (input == KEY_UP_P2 || input == KEY_DOWN_P2 || input == KEY_LEFT_P2 || input == KEY_RIGHT_P2);
    switch (input) {
        case KEY_UP_P1: case KEY_UP_P2:
            if (players[p].facingDirection != 1) players[p].facingDirection = 0;
            break;
        case KEY_DOWN_P1: case KEY_DOWN_P2:
            if (players[p].facingDirection != 0) players[p].facingDirection = 1;
            break;
        case KEY_LEFT_P1: case KEY_LEFT_P2:
            if (players[p].facingDirection != 3) players[p].facingDirection = 2;
            break;
        case KEY_RIGHT_P1: case KEY_RIGHT_P2:
            if (players[p].facingDirection != 2) players[p].facingDirection = 3;
            break;
        default:
            break;
    }
}


// Returns 0 if no collisions, returns 1 if player1 collides with an existing trail in the game matrix,
// returns 2 if player2 collides with an existing trail in the game matrix
int checkCollision(PlayerType* players, int playerCount, int gameStatus[HEIGHT][WIDTH]){
    for(int i = 0; i<playerCount; i++){
        if(gameStatus[players[i].currentY][players[i].currentX] || players[i].currentX < 0 || players[i].currentX >= WIDTH || players[i].currentY < 0 || players[i].currentY >= HEIGHT){
            return i+1;
            //si no, chequear que no se choque con el otro
        }else if(playerCount > 1 && players[0].currentX == players[1].currentX && players[0].currentY == players[1].currentY){
            return 3;
        }
    }
    return 0;
}

void eliminator(int playerCount){
    clear_scr();
    PlayerType player1 = {WIDTH/3, HEIGHT/3, RED, 3, {WIDTH/3, HEIGHT/3}};
    PlayerType player2 = {WIDTH/2, HEIGHT/2, BLUE, 0, {WIDTH/2, HEIGHT/2}};
    PlayerType players[2] = {player1, player2};

    //0 is empty, 1 is player1 trail, 2 is player2 trail
    //TODO, podemos guardar solo - 0 o 1
    prints("Eliminator game started\n", MAX_BUFF);
    int gameStatus[HEIGHT][WIDTH];
    for(int i = 0; i<HEIGHT; i++){
        for(int j = 0; j<WIDTH; j++){
            gameStatus[i][j] = 0;
        }
    }
    int loser = 0;

    while(!loser){
        checkInput(players);
        draw(players, playerCount);
        updateGameStatus(players, playerCount, gameStatus);
        updatePlayerPosition(players, playerCount, gameStatus);
        loser = checkCollision(players, playerCount, gameStatus);
        wait(50);
    }
    gameOverScreen(loser);
    return;
}

void gameOverScreen(int loser){
    drawRectangle(0, 0, get_scrWidth() / 2, get_scrHeight() / 8, BLACK);
    if (loser == 3){
        prints("\nGame Over. Both players lost!\nPress 'space' to play again or 'q' to quit", MAX_BUFF);
    }else{
        prints("\nGame Over. Player ", MAX_BUFF);
        printc(loser + '0');
        prints("LOST!\nPress 'space' to play again or 'q' to quit", MAX_BUFF);
    }
    char c;
    while ( ((c = getChar()) != 'q') && (c != ' ')){
        continue;
    }
    if(c == ' '){
        eliminator(2);
    }
    clear_scr();
    return;
}

void updatePlayerPosition(PlayerType* players, int playerCount, int gameStatus[HEIGHT][WIDTH]){
    for (int i=0; i<playerCount; i++){
        switch (players[i].facingDirection) {
            case 0: // Up
                players[i].currentY--;
                break;
            case 1: // Down
                players[i].currentY++;
                break;
            case 2: // Left
                players[i].currentX--;
                break;
            case 3: // Right
                players[i].currentX++;
                break;
            default:
                break;
        }
    }
}

void updateGameStatus(PlayerType* players, int playerCount, int gameStatus[HEIGHT][WIDTH]){
    for (int i=0; i<playerCount; i++){
        switch (players[i].facingDirection) {
            case 0: // Up
                gameStatus[players[i].currentY][players[i].currentX] = 1;
                break;
            case 1: // Down
                gameStatus[players[i].currentY][players[i].currentX] = 1;
                break;
            case 2: // Left
                gameStatus[players[i].currentY][players[i].currentX] = 1;
                break;
            case 3: // Right
                gameStatus[players[i].currentY][players[i].currentX] = 1;
                break;
            default:
                break;
        }
    }
}