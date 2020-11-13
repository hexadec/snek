/**
 * \file fileio.c
 * \author hexadec
 * \brief This file is responsible for handling file IO operations
 *          such as saving the user's score and retrieving the highscore
 */

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "fileio.h"

#define BUFFER_SIZE 100

static const char scores_file[] = "scores.txt";

/**
 * Saves player's score
 * @param name player name
 * @param score score achieved in this round
 * @return result of fprintf, -1 on failure
 */
int saveScore(char * name, int score) {
    FILE * file = fopen(scores_file, "a");
    if (file == NULL)
        return -1;
    if (strlen(name) > 80)
        name[81] = '\0';
    int result = fprintf(file, "%s,%d\n", name, score);
    fclose(file);
    return result;
}

/**
 * Retrieves the highscore of a given player from the scores file
 * @param name player name
 * @return highscore of player, 0 if not found, -1 * (error code) on error
 */
int getHighscore(char * name) {
    FILE * file = fopen(scores_file, "r");
    char buffer[BUFFER_SIZE];
    int score = 0;
    while (fgets(buffer, BUFFER_SIZE, file) != NULL) {
        char * separator = strchr(buffer, ',');
        if (separator == NULL)
            return -EBADF;
        *separator = '\0';
        if (strcmp(buffer, name) == 0) {
            int line_score;
            int result = sscanf(separator + 1, "%d", &line_score);
            if (result == 1 && line_score > score)
                score = line_score;
        }
    }
    return score;
}
