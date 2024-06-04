#include <libc.h>
#include <snake.h>

int gameEnded;

int startGame(int players){
    //clearScSaveBuf(); //ver que hace
    int valid = 1;  
    switch(players){
        case 1:
            singlePlayer();
            valid = 0;
            break;
        case 2:
            doublePlayers();
            break;
        default:
            valid = 0;
            break;
    }
    return valid;
}

void paintBackground(char board[HEIGHT][WIDTH], player * playerONE, player * playerTWO){
    int colorToPrint;
    for (int i = 0; i<HEIGHT; i++){
        for(int j = 0; j<WIDTH; j++){
            if (board[i][j] == playerONE->character){
                colorToPrint = playerONE->gender;
            }
            else if (playerTWO != NULL && board[i][j] == playerTWO->character){
                colorToPrint = playerTWO->gender;
            }
            else if (board[i][j] == FOOD){
                 colorToPrint = FRUIT;
            }
            else {
                if ((i+j) % 2 == 0){
                    colorToPrint = LIGHT_BACKGROUND;
                }else {
                    colorToPrint = DARK_BACKGROUND;
                }
            }
            drawSquare(SIZE, i*SIZE, j*SIZE, colorToPrint);
        }
    }
}

void Snake(char board[HEIGHT][WIDTH], player * player, int color, char character, char DIR, int posX, int posY){
    player->head.x = posX;
    player->head.y = posY;
    player->move = DIR;
    player->gender = color;
    player->character = character;
    player->length = 1;
    player->flag = 1;
    player->score = 0;
    for (int i = 0; i< HEIGHT; i++){
        for (int j=0; j<WIDTH; j++)
            board[i][j] = ' ';
    }

    for (int i = 0; i<player->length; i++){
        player->body[i].x = player->head.x;
        player->body[i].y = player->head.y - (i + 1);
        board[player->body[i].x][player->body[i].y] = character;
    }
    board[player->head.x][player->head.y] = player->character;
}


void action(char board[HEIGHT][WIDTH], player* playerONE, player * playerTWO,int *gameEnded){
    Point prevHead = playerONE->head;
    switch (playerONE->move) {
        case UP:
        case UP2:
            playerONE->head.y--;
            break;
        case DOWN:
        case DOWN2:
            playerONE->head.y++;
            break;
        case LEFT:
        case LEFT2:
            playerONE->head.x--;
            break;
        case RIGHT:
        case RIGHT2:
            playerONE->head.x++;
            break;
    }
    if (playerTWO != NULL){
        if (playerONE->head.x == playerTWO->head.x && playerONE->head.y == playerTWO->head.y){
            *gameEnded = 1;
            playerONE->flag = 0;
            playerTWO->flag = 0;
        }else {
            for (int i = 0; i < playerTWO->length; i++) {
                if (playerONE->head.x == playerTWO->body[i].x && playerONE->head.y == playerTWO->body[i].y) {
                    *gameEnded = 1;
                    playerONE->flag = 0;
                    playerTWO->flag = 0;
                }
            }
        }
       
    }
    for (int i = 0; i < playerONE->length ; i++){
        if (playerONE->head.x == playerONE->body[i].x && playerONE->head.y == playerONE->body[i].y){
            *gameEnded = 1;
            playerONE->flag = 0;
        }
    }
    if (playerONE->head.x == -1 || playerONE->head.x >= HEIGHT-1 || playerONE->head.y == -1 || playerONE->head.y >= WIDTH ){
        *gameEnded = 1;
        playerONE->flag = 0;
    }
     if (board[playerONE->head.x][playerONE->head.y] == FOOD){
        playerONE->length++; 
        playSound(100);
        playerONE->score++;
        //mute();
        generateFood(board);
    }
    //actualizo el cuerpo mientras se mueve
    for (int i = playerONE->length - 1; i >= 0; i--) {
        playerONE->body[i + 1] = playerONE->body[i];
    }
    // muevo la cabeza
    playerONE->body[0] = prevHead;
    // Update board with new positions
    board[playerONE->head.x][playerONE->head.y] = playerONE->character;
    for (int i = 0; i < playerONE->length; i++) {
        board[playerONE->body[i].x][playerONE->body[i].y] = playerONE->character;
    }
    //borro la ultima posicion de la cola
    Point lastMove = playerONE->body[playerONE->length];
    board[lastMove.x][lastMove.y] = ' ';
}

void UpdateMove(char key, player*player, char moveUp, char moveDown, char moveLeft, char moveRight ){
     if(key ==  'q' ){
            gameEnded = 1;
        }
        if(!((player->move == moveUp && key == moveDown) || (player->move == moveDown && key == moveUp) || (player->move == moveLeft && key == moveRight) || (player->move == moveRight && key == moveLeft))){
            if(key == moveUp ||key == moveLeft ||key == moveDown || key == moveRight){
                player->move = key;
            }
        }
    
}

void generateFood(char board[HEIGHT][WIDTH]){
    int randy = randInt(0, WIDTH - 1);
    int randx = randInt(0, HEIGHT - 1);
    board[randx][randy] = FOOD;
}

// void gameOver(){
//     printInScreen("GAME OVER", 9);
// }

//SINGLE PLAYER

void singlePlayer(){
    char board[HEIGHT][WIDTH];
    gameEnded = 0;

    player playerONE;
    int posX = WIDTH/2;
    int posY = HEIGHT/2;
    Snake(board, &playerONE, COMP1, SNAKE_ONE, RIGHT, posX, posY);
    generateFood(board);
    paintBackground(board,&playerONE, NULL);
    char key = RIGHT;
    while (!gameEnded){
        key = readChar();
        UpdateMove(key, &playerONE, UP, DOWN, LEFT, RIGHT);
        action(board, &playerONE, NULL,&gameEnded);
        paintBackground(board, &playerONE, NULL);
        waitSec();
    }
    backToTerm();
    
}

/// 2 PLAYERS

void doublePlayers(){
    char board[HEIGHT][WIDTH];
    gameEnded = 0;

    player playerONE;
    player playerTWO;
    int posX = WIDTH/2;
    int posY = HEIGHT/2;
    Snake(board, &playerONE, COMP1, SNAKE_ONE, RIGHT, posX, posY);
    posX = 8;
    posY = 8;
    Snake(board, &playerTWO, COMP2, SNAKE_TWO, DOWN2, posX, posY);
    generateFood(board);
    paintBackground(board,&playerONE, &playerTWO);
    char keyONE = RIGHT;
    char keyTWO = DOWN2;
    while (!gameEnded){
        keyONE = readChar();
        UpdateMove(keyONE, &playerONE, UP, DOWN, LEFT, RIGHT);
        action(board, &playerONE, &playerTWO,&gameEnded);
        keyTWO = readChar();
        UpdateMove(keyTWO, &playerTWO, UP2, DOWN2, LEFT2, RIGHT2);
        action(board, &playerTWO, &playerONE,&gameEnded);
        paintBackground(board, &playerONE, &playerTWO);
        waitSec();
    }
    backToTerm();
}