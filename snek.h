//
// Created by hexadec on 11/3/20.
//

#ifndef SNEK_SNEK_H
#define SNEK_SNEK_H

typedef enum {UP, DOWN, LEFT, RIGHT} Direction;

typedef struct {
    int score;
    int xsize, ysize;
    Direction direction;
} Snek;

#endif //SNEK_SNEK_H
