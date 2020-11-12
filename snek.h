//
// Created by hexadec on 11/3/20.
//

#ifndef SNEK_SNEK_H
#define SNEK_SNEK_H

#include "linkedlist.h"

typedef enum {UP, DOWN, LEFT, RIGHT} Direction;

typedef struct Point {
    int x, y;
} Point;

typedef struct {
    int score, highscore;
    Direction direction;
    Point * food;
    LinkedList * snake;
    char * player_name;
} Snek;

#endif //SNEK_SNEK_H
