#include <ncurses.h>
#include <stdlib.h>
#include "snek.h"
#include "screen.h"

int main() {
    initializeScreen();
    Snek snek;
    snek.highscore = 1000;
    snek.score = 100;
    snek.snake = createLinkedList();
    for (int i = 0; i < 7; i++) {
        Point * p = malloc(sizeof(Point));
        p->x = i + 4;
        p->y = 12;
        snek.snake->add(snek.snake, p);
    }
    drawGame(&snek);
    getch();
    closeScreen();
    return 0;
}
