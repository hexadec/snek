/**
 * This file handles all graphic operations and also handles user inputs
 * in order to make the project structure as modular as possible.
 * The game is displayed on the terminal where I/O operations are handled by wide-ncurses,
 * except for the terminal resize event, that exits the program.
 * Wide-ncurses is necessary to support shading characters and the food of the snake.
 * \file screen.c
 * \author hexadec
 * \brief This file is responsible for graphics and user interaction
 */

#include <ncursesw/curses.h>
#include <string.h>
#include <time.h>
#include <locale.h>
#include <signal.h>
#include <stdlib.h>
#include "screen.h"
#include "snek.h"

static void drawFrame();
static void drawSnake();
static void drawFood();
static void windowResizeHandler(__attribute__((unused)) int);

static WINDOW * window;
static int rows, columns;

/**
 * @brief \p enum storing indices of \p COLOR_PAIR -s
 */
enum TEXT_FORMATS {
    /** @brief White text, black background*/
    WHITE_BLACK,
    /** @brief Red text, black background*/
    RED_BLACK,
    /** @brief Green text, black background*/
    GREEN_BLACK,
    /** @brief Black text (darkgray), black background*/
    BLACK_BLACK
};

const static Snek * snek;

/**
 * Sets up necessary terminal configuration for the game,
 * Initializes ncurses, Sets terminal resize event handler
 * @brief prepares the terminal for the game
 * @param game hold all important game parameters
 */
void initializeScreen(const Snek * game) {
    snek = game;
    setlocale(LC_ALL, "");
    window = initscr();
    noecho();
    curs_set(0);
    getmaxyx(window, rows, columns);
    keypad(window, true);
    start_color();
    init_pair(WHITE_BLACK, COLOR_WHITE, COLOR_BLACK);
    init_pair(RED_BLACK, COLOR_RED, COLOR_BLACK);
    init_pair(GREEN_BLACK, COLOR_GREEN, COLOR_BLACK);
    init_pair(BLACK_BLACK, COLOR_BLACK, COLOR_BLACK);
    static struct sigaction signal_handler;
    memset(&signal_handler, 0, sizeof(struct sigaction));
    signal_handler.sa_handler = windowResizeHandler;
    sigaction(SIGWINCH, &signal_handler, NULL);
}

/**
 * @brief Handles window resize event using SIGWINCH
 * @param signal code of received signal (unused attribute is necessary because func.
 * pointers aren't recognised correctly)
 */
static void windowResizeHandler(__attribute__((unused)) int signal) {
    if (signal == SIGWINCH) {
        endwin();
        printf("Game aborted due to terminal resize\n");
        endGame(snek);
        exit(-1);
    }
}

/**
 * @brief Closes ncurses sessions, flushes characters in input queue
 */
void closeScreen() {
    flushinp();
    endwin();
}

/**
 * @brief Draws the current state of the game on the terminal
 */
void drawGame() {
    erase();
    drawFrame(snek);
    drawFood(snek);
    drawSnake(snek);
    refresh();
}

/**
 * @brief Returns the number of terminal rows
 * @return number of terminal rows
 */
int getRows() {
    return rows;
}

/**
 * @brief Returns the number of terminal columns
 * @return number of terminal columns
 */
int getColumns() {
    return columns;
}

/**
 * @brief Reads a character from the screen in a non-blocking way
 * @param timeout_ms milliseconds to wait before returning if no key has been pressed
 * @return keycode, or -1 if no button was pressed
 */
int readCharacter(long timeout_ms) {
    timeout(timeout_ms);
    int key = wgetch(window);
    if (key == ERR)
        return -1;
    flushinp();
    return key;
}

/**
 * @brief Prints an error message to the standard error output
 * @param error string to print
 */
void print_error(const char * error) {
    perror(error);
}

/**
 * @brief Draws the game-over screen
 */
void drawGameOver() {
    char game_over[] = "GAME OVER";
    attron(A_BOLD);
    for (int i = 0; i < 6; i++) {
        attron(i % 2 == 0 ? COLOR_PAIR(BLACK_BLACK) : COLOR_PAIR(RED_BLACK));
        mvaddstr((int) (rows / 2), (int) (columns / 2 - strlen(game_over) / 2), game_over);
        attroff(i % 2 == 0 ? COLOR_PAIR(BLACK_BLACK) : COLOR_PAIR(RED_BLACK));
        refresh();
        nanosleep(&(const struct timespec){0, 5E8}, NULL);
    }
    attroff(A_BOLD);
}

/**
 * @brief Draw the frame around the game field
 */
static void drawFrame() {
    attron(COLOR_PAIR(WHITE_BLACK));
    char status[50];
    sprintf(status, "SCORE%6d        HIGHSCORE%6d", snek->score, snek->highscore);
    wmove(window, 0, (int) (columns / 2 - strlen(status) / 2));
    printw(status);
    for (int i = 0; i < columns; i++) {
        mvaddstr(1, i, "▒");
        mvaddstr(rows - 1, i, "▒");
    }
    for (int i = 2; i < rows - 1; i++) {
        mvaddstr(i, 0, "▒");
        mvaddstr(i, columns - 1, "▒");
    }
    attroff(COLOR_PAIR(WHITE_BLACK));
}

/**
 * @brief Draw the food of the snake
 */
static void drawFood() {
    attron(COLOR_PAIR(RED_BLACK));
    mvaddstr(snek->food->y, snek->food->x, "●");
    attroff(COLOR_PAIR(RED_BLACK));
}

/**
 * @brief Draw the snake itself
 */
static void drawSnake() {
    LinkedList * snake = snek->snake;
    snake->toStart(snake);
    attron(COLOR_PAIR(GREEN_BLACK));
    do {
        Point * point = snake->node->data;
        mvaddstr(point->y, point->x, "▓");
    } while (snake->next(snake));
    attroff(COLOR_PAIR(GREEN_BLACK));
}
