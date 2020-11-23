/**
 * \file fileio.c
 * \author hexadec
 * \brief This file is responsible for handling file IO operations
 *          such as saving the user's score and retrieving the highscore
 */

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include "fileio.h"
#include "debugmalloc.h"

/** @brief Buffer size for reading from file (greater than max line length) */
#define BUFFER_SIZE 100

/**
 * Returns a pointer to the Nick_Score item containing the minimum score.
 * If there are multiple, it will return the one with the smallest index.
 * @brief Finds the item with the minimum score (or smallest index)
 * @param toplist Nick_Score list to search in
 * @param size size of the Nick_Score list
 * @return pointer to the item with the smallest score.
 */
static Nick_Score * getMinimumScore(Nick_Score * toplist, int size);

/**
 * @brief Sorts the toplist in descending order by the scores using selection sort
 * @param toplist toplist to sort
 * @param size size of the toplist
 */
static void sortToplist(Nick_Score * toplist, int size);

static const char scores_file[] = "scores.txt";

int saveScore(char * name, int score) {
    FILE * file = fopen(scores_file, "a");
    if (file == NULL)
        return -1;
    if (strlen(name) > 80)
        name[81] = '\0';
    int result = fprintf(file, "%s,%d\n", name, score);
    if (fclose(file) != 0)
        return -1;
    return result;
}

int getHighscore(char * name) {
    FILE * file = fopen(scores_file, "r");
    if (file == NULL)
        return 0;
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

Nick_Score * getToplist(int toplist_size) {
    const int nick_max_size = NICK_MAX_LENGTH;
    FILE * file = fopen(scores_file, "r");
    Nick_Score * toplist = calloc(toplist_size, sizeof(Nick_Score));
    if (file == NULL) {
        errno = ENOENT;
        return NULL;
    }
    char buffer[BUFFER_SIZE];
    while (fgets(buffer, BUFFER_SIZE, file) != NULL) {
        int score = 0;
        char * separator = strchr(buffer, ',');
        if (separator == NULL) {
            errno = EBADF;
            return NULL;
        } else if (separator - buffer > nick_max_size) {
            separator = buffer + nick_max_size;
        }
        *separator = '\0';
        int line_score;
        int result = sscanf(separator + 1, "%d", &line_score);
        if (result == 1)
            score = line_score;
        Nick_Score * min = getMinimumScore(toplist, toplist_size);
        if (min->score <= score) {
            // Not the most efficient way, but good enough for us
            free(min->nick);
            min->nick = malloc((strlen(buffer) + 1) * sizeof(char));
            strcpy(min->nick, buffer);
            min->score = score;
        }
    }
    sortToplist(toplist, toplist_size);
    return toplist;
}

static Nick_Score * getMinimumScore(Nick_Score * toplist, int size) {
    int min = 0;
    for (int i = 0; i < size; i++) {
        if (toplist[i].score < toplist[min].score)
            min = i;
        if (toplist[i].score == 0)
            break;
    }
    return &toplist[min];
}

static void sortToplist(Nick_Score * toplist, int size) {
    //Selection sort
    for (int i = 0; i < size - 1; i++) {
        int rangeMaximum = i;
        for (int j = i; j < size; j++) {
            if (toplist[j].score > toplist[rangeMaximum].score)
                rangeMaximum = j;
        }
        if (rangeMaximum != i) {
            int temp_score = toplist[i].score;
            toplist[i].score = toplist[rangeMaximum].score;
            toplist[rangeMaximum].score = temp_score;

            char * temp_string = toplist[i].nick;
            toplist[i].nick = toplist[rangeMaximum].nick;
            toplist[rangeMaximum].nick = temp_string;
        }
    }
}
