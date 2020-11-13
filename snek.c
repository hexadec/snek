/**
 * \file snek.c
 * \author hexadec
 * \brief This file handles the logic of the game, and also contains the entry point of the program
 */

#include <stdlib.h>
#include <sys/random.h>
#include <time.h>
#include "snek.h"
#include "screen.h"
#include "debugmalloc.h"
#include "fileio.h"

bool stepGame(Snek *);
void addNewHead(Snek *);
void placeNewFood(Snek *);
void gameLoop(Snek *);
void initGame(Snek *);
bool isGameOver(const Snek *);
bool isPointInSnake(const Snek *, int, int, bool);

void mallocError(const Snek * snek);

/**
 * Entry point of the program
 * @return exit code
 */
int main() {
    Snek snek;
    initializeScreen(&snek);
    initGame(&snek);
    gameLoop(&snek);
    drawGameOver();
    readCharacter(-1);
    endGame(&snek);
    return 0;
}

/**
 * Initialises game: read highscore for given player, create necessary data structures
 * @param snek holds all important game parameters
 */
void initGame(Snek * snek) {
    snek->highscore = getHighscore("Józsi");
    snek->score = 1;
    snek->direction = UP;
    snek->food = NULL;
    snek->snake = createLinkedList();
    if (snek->snake == NULL) mallocError(NULL);
    snek->player_name = "Józsi";

    Point * first = malloc(sizeof(Point));
    if (first == NULL) mallocError(snek);
    first->x = getColumns() / 2;
    first->y = getRows() / 2;
    if (snek->snake->addFirst(snek->snake, first) == false) mallocError(snek);

    snek->food = malloc(sizeof(Point));
    if (snek->food == NULL) mallocError(snek);
    placeNewFood(snek);
}

/**
 * This function is responsible for controlling the game after it has started
 * It reads a control character and steps the game until an exit condition has been reached
 * @param snek holds all important game parameters
 * @returns when an exit condition had been met
 */
void gameLoop(Snek * snek) {
    struct timespec start, end;
    long remainder = 0;
    bool continue_game = true;
    do {
        if (!remainder)
            drawGame(snek);
        clock_gettime(CLOCK_MONOTONIC_RAW, &start);
        int dir = readCharacter(remainder == 0 ? 750 : remainder);
        switch (dir) {
            case 'w':
                if (snek->direction != DOWN)
                    snek->direction = UP;
                break;
            case 'a':
                if (snek->direction != RIGHT)
                    snek->direction = LEFT;
                break;
            case 's':
                if (snek->direction != UP)
                    snek->direction = DOWN;
                break;
            case 'd':
                if (snek->direction != LEFT)
                    snek->direction = RIGHT;
                break;
            case -1:
                //No button was pressed
                break;
            default:
                //A button other than a control button has been pressed
                //Go to next iteration, but allow steps to occur at the same time
                clock_gettime(CLOCK_MONOTONIC_RAW, &end);
                remainder = (remainder == 0 ? 750 : remainder) - ((end.tv_sec - start.tv_sec) * 1000L + (end.tv_nsec - start.tv_nsec) / (long) 1E6);
                continue;
        }
        remainder = 0;
        continue_game = stepGame(snek);
    } while (continue_game);
}

/**
 * Steps the game to its next state. This includes moving the snake and placing a new food
 * @param snek holds all important game parameters
 * @return false if an exit condition has been met, true otherwise
 */
bool stepGame(Snek * snek) {
    addNewHead(snek);
    if (isGameOver(snek)) return false;
    LinkedList * snake = snek->snake;
    snake->toStart(snake);
    Point * head = snake->node->data;
    if (head->x == snek->food->x && head->y == snek->food->y) {
        snek->score++;
        placeNewFood(snek);
    } else {
        snake->toEnd(snake);
        snake->removeItem(snake);
    }
    return true;
}

/**
 * Adds a new head to the snake to the direction specified
 * @param snek holds all important game parameters
 */
void addNewHead(Snek * snek) {
    LinkedList * snake = snek->snake;
    snake->toStart(snake);
    Point * head = snake->node->data;
    Point * new = malloc(sizeof(Point));
    if (new == NULL) mallocError(snek);
    switch (snek->direction) {
        case UP:
            new->x = head->x;
            new->y = head->y - 1;
            break;
        case DOWN:
            new->x = head->x;
            new->y = head->y + 1;
            break;
        case LEFT:
            new->x = head->x - 1;
            new->y = head->y;
            break;
        case RIGHT:
            new->x = head->x + 1;
            new->y = head->y;
            break;
    }
    if (!snake->addFirst(snake, new)) mallocError(snek);
}

/**
 * Places a new food in the game in a random position, on a block that is not occupied by the snake
 * @param snek holds all important game parameters
 */
void placeNewFood(Snek * snek) {
    int x, y;
    do {
        unsigned int coords[2];
        //Generate cryptographically secure random numbers (for fun) (syscall!)
        getrandom(&coords, 2 * sizeof(unsigned int), GRND_RANDOM);
        x = (int) (coords[0] % (getColumns() - 2) + 1);
        y = (int) (coords[1] % (getRows() - 3) + 2);
    } while (isPointInSnake(snek, x, y, false));
    snek->food->x = x;
    snek->food->y = y;
}

/**
 * Checks if an end-of-game condition has been met.
 * This includes the snake biting on its tail and hitting a wall.
 * @param snek holds all important game parameters
 * @return
 */
bool isGameOver(const Snek * snek) {
    LinkedList * snake = snek->snake;
    snake->toStart(snake);
    Point * head = snake->node->data;
    if (head->x < 1 || head->y < 2)
        return true;
    if (head->x > getColumns() - 2 || head->y > getRows() - 2)
        return true;
    return isPointInSnake(snek, head->x, head->y, true);
}

/**
 * Checks if a given point is occupied by the snake
 * @param snek holds all important game parameters
 * @param x column index
 * @param y row index
 * @param ignore_head do not check for x, y point in the head of the snake
 * @return true, if the point is inside the snake, false otherwise
 */
bool isPointInSnake(const Snek * snek, int x, int y, bool ignore_head) {
    LinkedList * snake = snek->snake;
    snake->toStart(snake);
    if (ignore_head) {
        if (!snake->hasNext(snake)) return false;
        else snake->next(snake);
    }
    do {
        Point * point = snake->node->data;
        if (point->x == x && point->y == y)
            return true;
    } while (snake->next(snake));
    return false;
}

/**
 * Finishes the game, frees memory
 * @param snek holds all important game parameters
 */
void endGame(const Snek * snek) {
    closeScreen();
    saveScore(snek->player_name, snek->score);
    dumpLinkedList(snek->snake);
    free(snek->food);
}

/**
 * Finishes the game, prints an error message
 * Frees all allocated memory, if there was any
 */
void mallocError(const Snek * snek){
    if (snek != NULL)
        endGame(snek);
    else
        closeScreen();
    print_error("Couldn't allocate memory\n");
    exit(-3);
}
