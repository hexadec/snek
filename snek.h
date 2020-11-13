//
// Created by hexadec on 11/3/20.
//

#ifndef SNEK_SNEK_H
#define SNEK_SNEK_H

#include "linkedlist.h"

typedef enum {UP, DOWN, LEFT, RIGHT} Direction;

/**
 * Structure to hold positions on a 2D plane
 */
typedef struct Point {
    int x, y;
} Point;

/**
 * Structure to hold all important parameters of the game
 */
typedef struct {
    int score, highscore;
    Direction direction;
    Point * food;
    LinkedList * snake;
    char * player_name;
} Snek;

void endGame(const Snek * snek);

#endif //SNEK_SNEK_H
