//
// Created by hexadec on 11/3/20.
//

#ifndef SNEK_SCREEN_H
#define SNEK_SCREEN_H

#include "snek.h"

void drawGame(const Snek * snek);
void initializeScreen();
void closeScreen();
int getRows();
int getColumns();
char readCharacter(int);
void print_error(const char *);

#endif //SNEK_SCREEN_H
