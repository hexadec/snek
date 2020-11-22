//
// Created by hexadec on 11/3/20.
//

#ifndef SNEK_SCREEN_H
#define SNEK_SCREEN_H

#include "snek.h"

/**
 * @brief Draws the current state of the game on the terminal
 */
void drawGame();

/**
 * Sets up necessary terminal configuration for the game,
 * Initializes ncurses, Sets terminal resize event handler
 * @brief prepares the terminal for the game
 * @param game hold all important game parameters
 */
void initializeScreen(Snek * snek);

/**
 * @brief Closes ncurses sessions, flushes characters in input queue
 */
void closeScreen();

/**
 * @brief Reads a character from the screen in a non-blocking way
 * @param timeout_ms milliseconds to wait before returning if no key has been pressed
 * @return keycode, or -1 if no button was pressed
 */
int readCharacter(long);

/**
 * @brief Prints an error message to the standard error output
 * @param error string to print
 */
void print_error(const char *);

/**
 * @brief Draws the game-over screen
 */
void drawGameOver();

/**
 * @brief Draws the toplist on the screen
 * @param toplist list containing nickname-score pairs (in \p Nick_Score structures)
 * @param size size of the \p toplist
 */
void drawToplist(Nick_Score * toplist, int size);

/**
 * @brief Asks the user for their nickname
 * @param username pointer to pointer-to-character-array
 */
void getNickname(char ** username);

/**
 * @brief Calculates the length of a valid UTF-8 encoded string
 * @param string UTF-8 encoded string
 * @return number of valid UTF-8 characters
 */
size_t strlenUTF8(char * string);

/**
 * @brief Draws a yes/no style question dialog on the screen
 * @param question String to draw as the question
 * @param optTrue String to draw as the \p true option
 * @param optFalse String to draw as the \p false option
 * @return \p true if \p optTrue was selected, \p false if \p optFalse was selected
 */
bool drawQuestionDialog(char * question, char * optTrue, char * optFalse);

#endif //SNEK_SCREEN_H
