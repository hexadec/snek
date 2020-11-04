//
// Created by hexadec on 11/3/20.
//

#include <ncursesw/curses.h>
#include <string.h>
#include <locale.h>
#include "screen.h"
#include "snek.h"

static void drawFrame(const Snek *);

static WINDOW * window;
static int rows, cols;

void initializeScreen() {
    setlocale(LC_ALL, "");
    window = initscr();
    getmaxyx(window, rows, cols);
    start_color();
    init_pair(0, COLOR_WHITE, COLOR_BLACK);
    init_pair(1, COLOR_RED, COLOR_BLACK);
}

void closeScreen() {
    endwin();
}

void drawGame(const Snek * snek) {
    drawFrame(snek);
    refresh();
}

static void drawFrame(const Snek * snek) {
    attron(COLOR_PAIR(0));
    char status[50];
    sprintf(status, "SCORE%6d        HIGHSCORE%6d", snek->score, snek->highscore);
    wmove(window, 0, (int) (cols / 2 - strlen(status) / 2));
    printw(status);
    for (int i = 0; i < cols; i++) {
        mvaddstr(1, i, "▒");
        mvaddstr(rows - 1, i, "▒");
    }
    for (int i = 2; i < rows - 1; i++) {
        mvaddstr(i, 0, "▒");
        mvaddstr(i, cols - 1, "▒");
    }
    attroff(COLOR_PAIR(0));
}
