//
// Created by hexadec on 11/12/20.
//

#ifndef SNEK_FILEIO_H
#define SNEK_FILEIO_H

#include "snek.h"

/**
 * @brief Saves player's score
 * @param name player name
 * @param score score achieved in this round
 * @return result of fprintf, -1 on failure
 */
int saveScore(char *, int);

/**
 * @brief Retrieves the highscore of a given player from the scores file
 * @param name player name
 * @return highscore of player, 0 if not found, -1 * (error code) on error
 */
int getHighscore(char *);

/**
 * Reads the toplist from the scores file. This method creates a \p dynamically allocated
 * Nick_Score list of the desired size (set by \p toplist_size ).
 * Names in the array are also dynamically allocated.
 * @brief Returns a toplist containing the highest scores
 * @param toplist_size how many items should the toplist contain
 * @return toplist pointer to the toplist of desired size
 */
Nick_Score * getToplist(int);

#endif //SNEK_FILEIO_H
