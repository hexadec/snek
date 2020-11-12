//
// Created by hexadec on 11/3/20.
//

#include <ncursesw/curses.h>
#include <string.h>
#include <time.h>
#include <locale.h>
#include "screen.h"
#include "snek.h"

static void drawFrame(const Snek *);
static void drawSnake(const Snek *);
static void drawFood(const Snek *);

static WINDOW * window;
static int rows, columns;
enum TEXT_FORMATS {
        WHITE_BLACK, RED_BLACK, GREEN_BLACK, BLACK_BLACK
};

void initializeScreen() {
    setlocale(LC_ALL, "");
    window = initscr();
    noecho();
    curs_set(0);
    getmaxyx(window, rows, columns);
    start_color();
    init_pair(WHITE_BLACK, COLOR_WHITE, COLOR_BLACK);
    init_pair(RED_BLACK, COLOR_RED, COLOR_BLACK);
    init_pair(GREEN_BLACK, COLOR_GREEN, COLOR_BLACK);
    init_pair(BLACK_BLACK, COLOR_BLACK, COLOR_BLACK);
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

static void drawFrame(const Snek * snek) {
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

static void drawFood(const Snek * snek) {
    attron(COLOR_PAIR(RED_BLACK));
    mvaddstr(snek->food->y, snek->food->x, "●");
    attroff(COLOR_PAIR(RED_BLACK));
}

static void drawSnake(const Snek * snek) {
    LinkedList * snake = snek->snake;
    snake->toStart(snake);
    attron(COLOR_PAIR(GREEN_BLACK));
    do {
        Point * point = snake->node->data;
        mvaddstr(point->y, point->x, "▓");
    } while (snake->next(snake));
    attroff(COLOR_PAIR(GREEN_BLACK));
}
