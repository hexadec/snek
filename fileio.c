//
// Created by hexadec on 11/12/20.
//

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "fileio.h"

#define BUFFER_SIZE 100

static const char scores_file[] = "scores.txt";

int saveScore(char * name, int score) {
    FILE * file = fopen(scores_file, "a");
    if (strlen(name) > 80)
        name[81] = '\0';
    int result = fprintf(file, "%s,%d\n", name, score);
    fclose(file);
    return result;
}

int getHighscore(char * name) {
    FILE * file = fopen(scores_file, "r");
    char buffer[BUFFER_SIZE];
    int score = 0;
    while (fgets(buffer, BUFFER_SIZE, file) != NULL) {
        char * separator = strchr(buffer, ',');
        if (separator == NULL)
            return EBADF;
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
