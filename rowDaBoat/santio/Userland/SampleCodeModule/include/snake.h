#ifndef SNAKE_H
#define SNAKE_H

#include <colors.h>

//ONE PLAYER
#define UP 'w'
#define DOWN 's'
#define LEFT 'a'
#define RIGHT 'd'
//SECONDPLAYER
#define UP2 'i'
#define DOWN2 'k'
#define LEFT2 'j'
#define RIGHT2 'l'
//SNAKE
#define SNAKE_ONE '1'
#define SNAKE_TWO '2'
#define FOOD 'f'

#define WIDTH 24
#define HEIGHT 32
#define SIZE 32

#define NULL ((void*)0)

typedef struct Point{
    int x;
    int y;
}Point;

typedef struct player{
    Point head;
    Point body[WIDTH*HEIGHT];
    int move;
    int gender; //rosa o celeste
    char character;
    int length;
    int flag;
    int score;
} player;

int startGame(int players);
void paintBackground(char board[HEIGHT][ WIDTH], player * playerONE, player * playerTWO);
void Snake(char board[HEIGHT][WIDTH], player * player, int color, char character, char DIR, int posX, int posY);
void UpdateMove(char key, player * player, char moveUp, char moveDown, char moveLeft, char moveRight );
void action(char board[HEIGHT][WIDTH], player * playerONE, player * playerTWO,int * gameEnded);
void generateFood(char board[HEIGHT][WIDTH]);
// void gameOver();
void singlePlayer();
void doublePlayers();
#endif