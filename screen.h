//
// Created by hexadec on 11/3/20.
//

#ifndef SNEK_SCREEN_H
#define SNEK_SCREEN_H

#include "snek.h"

void drawGame();
void initializeScreen(const Snek * snek);
void closeScreen();
int getRows();
int getColumns();
char readCharacter(long);
void print_error(const char *);
void drawGameOver();

#endif //SNEK_SCREEN_H
