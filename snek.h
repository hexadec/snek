//
// Created by hexadec on 11/3/20.
//

#ifndef SNEK_SNEK_H
#define SNEK_SNEK_H

#include "linkedlist.h"

typedef enum {UP, DOWN, LEFT, RIGHT} Direction;

typedef struct {
    int score, highscore;
    Direction * direction;
    LinkedList * snake;
} Snek;

#endif //SNEK_SNEK_H
