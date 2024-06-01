#ifndef ELIMINATOR_H
#define ELIMINATOR_H

#include <userlib.h>
#include <colors.h>

typedef struct Player PlayerType;

void draw(PlayerType* players, int playerCount);
void checkInput(PlayerType player1, PlayerType player2);
int checkCollision(PlayerType* players, int playerCount, int** gameStatus);
void eliminator(int playerCount);
void gameOverScreen(int loser);
void updatePlayerPosition(PlayerType* players, int playerCount, int** gameStatus);


#endif /* ELIMINATOR_H */