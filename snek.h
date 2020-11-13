//
// Created by hexadec on 11/3/20.
//

#ifndef SNEK_SNEK_H
#define SNEK_SNEK_H

#include "linkedlist.h"

typedef enum {UP, DOWN, LEFT, RIGHT} Direction;

/**
 * @brief Structure to hold positions in a 2D plane
 */
typedef struct Point {
    int x, y;
} Point;

/** @brief Structure to hold all important parameters of the game */
typedef struct {
    /** @brief Current score of the current player */
    int score;
    /** @brief Highscore of current player */
    int highscore;
    /** @brief Direction in which the snake is moving */
    Direction direction;
    /** @brief Position of the snake's food */
    Point * food;
    /** @brief \p LinkedList containing the positions of the snake */
    LinkedList * snake;
    /** @brief nickname of current player */
    char * player_name;
} Snek;

void endGame(const Snek * snek);

#endif //SNEK_SNEK_H
