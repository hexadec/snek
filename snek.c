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

/**
 * @brief Frees the memory used by the toplist
 * @param toplist Toplist containing nickname and score pairs
 * @param toplist_size Size of the toplist
 */
void freeToplist(Nick_Score *, int);

/**
 * Steps the game to its next state. This includes moving the snake and placing a new food
 * @brief Steps the game to its next state
 * @param snek holds all important game parameters
 * @return \p false if an exit condition has been met, \p true otherwise
 */
bool stepGame(Snek *);

/**
 * @brief Adds a new head to the snake to the direction specified
 * @param snek holds all important game parameters
 */
void addNewHead(Snek *);

/**
 * Places a new food in the game in a random position, on a block that is not occupied by the snake
 * @brief Places a new food in the game
 * @param snek holds all important game parameters
 */
void placeNewFood(Snek *);

/**
 * This function is responsible for controlling the game after it has started
 * It reads a control character and steps the game until an exit condition has been reached
 * @brief Manages all game actions
 * @param snek holds all important game parameters
 * @returns when an exit condition had been met
 */
void gameLoop(Snek *);

/**
 * Initialises game: read highscore for given player, create necessary data structures
 * @brief Initialises the game
 * @param snek holds all important game parameters
 */
void initGame(Snek *);

/**
 * Checks if an end-of-game condition has been met.
 * This includes the snake biting on its tail and hitting a wall.
 * @brief Checks if an end-of-game condition has been met.
 * @param snek holds all important game parameters
 * @return \p true if an end-of-game condition has been met, \p false otherwise
 */
bool isGameOver(const Snek *);

/**
 * @brief Checks if a given point is occupied by the snake
 * @param snek holds all important game parameters
 * @param x column index
 * @param y row index
 * @param ignore_head do not check for x, y point in the head of the snake
 * @return \p true if the point is inside the snake, \p false otherwise
 */
bool isPointInSnake(const Snek *, int, int, bool);

/**
 * Finishes the game, prints an error message
 * Frees all allocated memory, if there was any
 * @brief Used to finish the game after a malloc error
 * @param snek holds all important game parameters
 */
void mallocError(const Snek * snek);

/**
 * @brief Entry point of the program
 * @return exit code
 */
int main() {
    Snek snek;
    //Sets memory to zero -> all pointers will be NULL, avoids segfault on resize
    memset(&snek, 0, sizeof(Snek));
    initializeScreen(&snek);
    initGame(&snek);
    gameLoop(&snek);
    saveScore(snek.player_name, snek.score);
    drawGameOver();
    readCharacter(-1);

    if (drawQuestionDialog("Do you want to see the toplist?", "Yes", "No")) {
        int toplist_size = 10;
        Nick_Score *toplist = getToplist(toplist_size);
        drawToplist(toplist, toplist_size);
        freeToplist(toplist, toplist_size);
        readCharacter(-1);
    }

    endGame(&snek);
    return 0;
}
void initGame(Snek * snek) {
    getNickname(&(snek->player_name));
    if (snek->player_name == NULL) mallocError(NULL);

    snek->highscore = getHighscore(snek->player_name);
    snek->score = 1;
    snek->direction = UP;
    snek->snake = createLinkedList();
    if (snek->snake == NULL) mallocError(snek);

    Point * first = malloc(sizeof(Point));
    if (first == NULL) mallocError(snek);
    first->x = snek->game_size.x / 2;
    first->y = snek->game_size.y / 2;
    if (snek->snake->addFirst(snek->snake, first) == false) mallocError(snek);

    snek->food = malloc(sizeof(Point));
    if (snek->food == NULL) mallocError(snek);
    placeNewFood(snek);
}

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

void placeNewFood(Snek * snek) {
    int x, y;
    unsigned int coords[2];
    do {
        //Generate cryptographically secure random numbers (for fun) (syscall!)
        getrandom(&coords, 2 * sizeof(unsigned int), GRND_RANDOM);
        x = (int) (coords[0] % (snek->game_size.x - 2) + 1);
        y = (int) (coords[1] % (snek->game_size.y - 3) + 2);
    } while (isPointInSnake(snek, x, y, false));
    snek->food->x = x;
    snek->food->y = y;
}

bool isGameOver(const Snek * snek) {
    LinkedList * snake = snek->snake;
    snake->toStart(snake);
    Point * head = snake->node->data;
    if (head->x < 1 || head->y < 2)
        return true;
    if (head->x > snek->game_size.x - 2 || head->y > snek->game_size.y - 2)
        return true;
    return isPointInSnake(snek, head->x, head->y, true);
}

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

void freeToplist(Nick_Score * toplist, int toplist_size) {
    for (int i = 0; i < toplist_size; i++) {
        free(toplist[i].nick);
    }
    free(toplist);
}

void endGame(const Snek * snek) {
    closeScreen();
    dumpLinkedList(snek->snake);
    free(snek->food);
    free(snek->player_name);
}

void mallocError(const Snek * snek){
    if (snek != NULL)
        endGame(snek);
    else
        closeScreen();
    print_error("Couldn't allocate memory\n");
    exit(-3);
}
