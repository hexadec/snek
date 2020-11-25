/**
 * This file handles all graphic operations and also handles user inputs
 * in order to make the project structure as modular as possible.
 * The game is displayed on the terminal where I/O operations are handled by wide-ncurses,
 * except for the terminal resize event, that exits the program.
 * Wide-ncurses is necessary to support shading characters and the food of the snake.
 * \file screen.c
 * \author hexadec
 * \brief This file is responsible for graphics and user interaction
 */

#include <ncursesw/curses.h>
#include <string.h>
#include <time.h>
#include <locale.h>
#include <signal.h>
#include <stdlib.h>
#include "debugmalloc.h"
#include "screen.h"
#include "snek.h"

/**
 * @brief Draw the frame around the game field
 */
static void drawFrame();

/**
 * @brief Draw the snake itself
 */
static void drawSnake(bool);

/**
 * @brief Draw the food of the snake
 */
static void drawFood();

/**
 * Handles signal events, such as terminal resize, and \p SIGUSR1.
 * \p SIGUSR1 is sent if the terminal size is too small
 * @brief Handles signal events (\p SIGWINCH and \p SIGUSR1 )
 * @param signal code of received signal
 */
static void signalEventHandler(int);

static WINDOW * window;
const static Snek * snek;

/**
 * @brief \p enum storing indices of \p COLOR_PAIR -s
 */
enum TEXT_FORMATS {
    /** @brief White text, black background*/
    WHITE_BLACK,
    /** @brief Red text, black background*/
    RED_BLACK,
    /** @brief Green text, black background*/
    GREEN_BLACK,
    /** @brief Black text (darkgray), black background*/
    BLACK_BLACK,
    /** @brief Black text, white background*/
    BLACK_WHITE
};

void initializeScreen(Snek * game) {
    snek = game;
    setlocale(LC_ALL, "");
    window = initscr();
    noecho();
    curs_set(0);
    game->game_size = (Point){getmaxx(window), getmaxy(window)};
    if (game->game_size.x < 35 || game->game_size.y < 8)
        //Too small terminal
        signalEventHandler(SIGUSR1);
    keypad(window, true);
    start_color();
    init_pair(WHITE_BLACK, COLOR_WHITE, COLOR_BLACK);
    init_pair(RED_BLACK, COLOR_RED, COLOR_BLACK);
    init_pair(GREEN_BLACK, COLOR_GREEN, COLOR_BLACK);
    init_pair(BLACK_BLACK, COLOR_BLACK, COLOR_BLACK);
    init_pair(BLACK_WHITE, COLOR_BLACK, COLOR_WHITE);
    static struct sigaction signal_handler;
    memset(&signal_handler, 0, sizeof(struct sigaction));
    signal_handler.sa_handler = signalEventHandler;
    sigaction(SIGWINCH, &signal_handler, NULL);
}

static void signalEventHandler(int signal) {
    switch (signal) {
        case SIGWINCH:
            endwin();
            printf("Game aborted due to terminal resize\n");
            endGame(snek);
            exit(-1);
            break;
        case SIGUSR1:
            endwin();
            printf("Terminal size too small, aborting\n");
            endGame(snek);
            exit(-1);
            break;
        default:
            break;
    }
}

void closeScreen() {
    flushinp();
    endwin();
}

void drawGame() {
    erase();
    drawFrame();
    drawFood();
    drawSnake(false);
    refresh();
}

int readCharacter(long timeout_ms) {
    timeout(timeout_ms);
    int key = wgetch(window);
    if (key == ERR)
        return -1;
    flushinp();
    return key;
}

void print_error(const char * error) {
    perror(error);
}

void drawGameOver() {
    char game_over[] = "GAME OVER";
    attron(A_BOLD);
    for (int i = 0; i < 10; i++) {
        drawSnake(i % 2 == 0);
        attron(i % 2 == 0 ? COLOR_PAIR(BLACK_BLACK) : COLOR_PAIR(RED_BLACK));
        mvaddstr((int) (getmaxy(window) / 2), (int) (getmaxx(window) / 2 - strlen(game_over) / 2), game_over);
        attroff(i % 2 == 0 ? COLOR_PAIR(BLACK_BLACK) : COLOR_PAIR(RED_BLACK));
        refresh();
        nanosleep(&(struct timespec){0, 4E8}, NULL);
    }
    attroff(A_BOLD);
    mvprintw(getmaxy(window) - 1, 0, "Press any key to continue");
}

static void drawFrame() {
    attron(COLOR_PAIR(WHITE_BLACK));
    char status[50];
    sprintf(status, "SCORE%6d        HIGHSCORE%6d", snek->score, snek->highscore);
    wmove(window, 0, 0);
    printw(snek->player_name);
    wmove(window, 0, (int) (getmaxx(window) / 2 - strlen(status) / 2));
    printw(status);
    for (int i = 0; i < getmaxx(window); i++) {
        mvaddstr(1, i, "▒");
        mvaddstr(getmaxy(window) - 1, i, "▒");
    }
    for (int i = 2; i < getmaxy(window) - 1; i++) {
        mvaddstr(i, 0, "▒");
        mvaddstr(i, getmaxx(window) - 1, "▒");
    }
    attroff(COLOR_PAIR(WHITE_BLACK));
}

static void drawFood() {
    attron(COLOR_PAIR(RED_BLACK));
    mvaddstr(snek->food->y, snek->food->x, "●");
    attroff(COLOR_PAIR(RED_BLACK));
}

static void drawSnake(bool ghost) {
    LinkedList * snake = snek->snake;
    snake->toStart(snake);
    attron(COLOR_PAIR(GREEN_BLACK));
    do {
        Point * point = snake->node->data;
        mvaddstr(point->y, point->x, ghost ? "░" : "▓");
    } while (snake->next(snake));
    attroff(COLOR_PAIR(GREEN_BLACK));
}

void getNickname(char ** username) {
    const int nick_max_size = NICK_MAX_LENGTH;
    int columns = getmaxx(window);
    echo();
    *username = malloc((nick_max_size + 1) * sizeof(char));
    mvaddstr(getmaxy(window) / 2, columns / 2 - (columns > 30 ? 8 : columns / 4), "Nickname?  ");
    keypad(window, false);
    getnstr(*username, nick_max_size);
    if (strlen(*username) < 1)
        strcpy(*username, "anonymous");
    noecho();
    keypad(window, true);
}

bool drawQuestionDialog(char * question, char * optTrue, char * optFalse) {
    erase();
    size_t question_length = strlenUTF8(question);
    size_t opt_true_length = strlenUTF8(optTrue);
    size_t opt_false_length = strlenUTF8(optFalse);
    int centerx = getmaxx(window) / 2;
    int centery = getmaxy(window) / 2 - 4 / 2;
    attron(A_BOLD);
    mvprintw(centery, centerx - question_length / 2, question);
    attroff(A_BOLD);
    unsigned selection = 0;
    int c = 0;
    keypad(window, true);
    do { //Move the selection between the two options
        if (c == KEY_UP || c == KEY_DOWN)
            selection++;
        attron(selection % 2 == 0 ? COLOR_PAIR(BLACK_WHITE) : COLOR_PAIR(WHITE_BLACK));
        mvprintw(centery + 2, centerx - opt_true_length / 2, optTrue);
        attroff(selection % 2 == 0 ? COLOR_PAIR(BLACK_WHITE) : COLOR_PAIR(WHITE_BLACK));
        attron(selection % 2 == 1 ? COLOR_PAIR(BLACK_WHITE) : COLOR_PAIR(WHITE_BLACK));
        mvprintw(centery + 3, centerx - opt_false_length / 2, optFalse);
        attroff(selection % 2 == 1 ? COLOR_PAIR(BLACK_WHITE) : COLOR_PAIR(WHITE_BLACK));
    } while ((c = getch()) != '\n'); // Until the user presses enter
    return selection % 2 == 0;
}

size_t strlenUTF8(char * str) {
    //https://en.wikipedia.org/wiki/UTF-8#Encoding
    size_t count = 0;
    if (str == NULL)
        return 0;
    while (*str != '\0') {
        if ((unsigned char)(*str) < 0x80) // ASCII-block
            count++;
        else if ((unsigned char)(*str) <= 0xDF && (unsigned char)(*str) >= 0xC2) { // Characters with two byte representation
            if (((unsigned char)(*(str + 1)) & 0xC0u) == 0x80)
                count++;
        } else if ((unsigned char)(*str) <= 0xEF && (unsigned char)(*str) >= 0xE0) { // Characters with three byte representation
            if (((unsigned char)(*(str + 1)) & 0xC0u) == 0x80 && ((unsigned char)(*(str + 2)) & 0xC0u) == 0x80)
                count++;
        } else if ((unsigned char)(*str) <= 0xF4 && (unsigned char)(*str) >= 0xF0) { // Characters with four byte representation
            if (((unsigned char)(*(str + 1)) & 0xC0u) == 0x80 && ((unsigned char)(*(str + 2)) & 0xC0u) == 0x80 && ((unsigned char)(*(str + 3)) & 0xC0u) == 0x80)
                count++;
        }
        str++;
    }
    return count;
}

void drawToplist(Nick_Score * toplist, int size) {
    erase();
    int centerx = getmaxx(window) / 2 - 20 / 2;
    int centery = getmaxy(window) / 2 - size / 2;
    mvprintw(centery - 2, centerx, "TOP %d", size);
    // ─ is three bytes long, therefore it would mess with both normal
    // and wide char functions, so do it the easy way (20 pcs)
    mvprintw(centery - 1, centerx, "────────────────────");
    for (int i = 0; i < size; i++) {
        size_t nicklen = strlenUTF8(toplist[i].nick);
        if (nicklen == 0)
            break;
        if (i < 3)
            attron(A_BOLD);
        mvprintw(centery++, centerx, "%s%*d", toplist[i].nick, 20 - nicklen,toplist[i].score);
        if (i < 3)
            attroff(A_BOLD);
    }
    mvprintw(getmaxy(window) - 1, 0, "Press any key to quit");
    refresh();
}
