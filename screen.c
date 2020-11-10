//
// Created by hexadec on 11/3/20.
//

#include <ncursesw/curses.h>
#include <string.h>
#include <locale.h>
#include "screen.h"
#include "snek.h"

static void drawFrame(const Snek *);
static void drawSnake(const Snek *);
static void drawFood(const Snek *);

static WINDOW * window;
static int rows, columns;

void initializeScreen() {
    setlocale(LC_ALL, "");
    window = initscr();
    noecho();
    curs_set(0);
    getmaxyx(window, rows, columns);
    start_color();
    init_pair(0, COLOR_WHITE, COLOR_BLACK);
    init_pair(1, COLOR_RED, COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
}

void closeScreen() {
    endwin();
}

void drawGame(const Snek * snek) {
    erase();
    drawFrame(snek);
    drawFood(snek);
    drawSnake(snek);
    refresh();
}

int getRows() {
    return rows;
}

int getColumns() {
    return columns;
}

char readCharacter(int timeout_ms) {
    timeout(timeout_ms);
    return getch();
}

void print_error(const char * error) {
    perror(error);
}

static void drawFrame(const Snek * snek) {
    attron(COLOR_PAIR(0));
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
    attroff(COLOR_PAIR(0));
}

static void drawFood(const Snek * snek) {
    attron(COLOR_PAIR(1));
    mvaddstr(snek->food->y, snek->food->x, "●");
    attroff(COLOR_PAIR(1));
}

static void drawSnake(const Snek * snek) {
    LinkedList * snake = snek->snake;
    snake->toStart(snake);
    attron(COLOR_PAIR(2));
    do {
        Point * point = snake->node->data;
        mvaddstr(point->y, point->x, "▓");
    } while (snake->next(snake));
    attroff(COLOR_PAIR(2));
}
