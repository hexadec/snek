#include <ncurses.h>
#include "snek.h"
#include "screen.h"

int main() {
    initializeScreen();
    Snek snek;
    snek.highscore = 1000;
    snek.score = 100;
    drawGame(&snek);
    getch();
    closeScreen();
    return 0;
}
