#include <stdlib.h>
#include <time.h>
#include "snek.h"
#include "screen.h"

bool stepGame(Snek *);
void addNewHead(Snek *);
void placeNewFood(Snek *);
bool isGameOver(const Snek *);
bool isPointInSnake(const Snek *, int, int, bool);

void mallocError();

int main() {
    srand(time(0));
    initializeScreen();
    Snek snek;
    snek.highscore = 1000;
    snek.score = 1;
    snek.snake = createLinkedList();
    snek.direction = UP;
    Point * first = malloc(sizeof(Point));
    first->x = getColumns() / 2;
    first->y = getRows() / 2;
    snek.snake->addFirst(snek.snake, first);
    if (snek.snake == NULL) mallocError();
    snek.food = malloc(sizeof(Point));
    placeNewFood(&snek);
    do {
        drawGame(&snek);
        char dir = readCharacter(750);
        switch (dir) {
            case 'w':
                if (snek.direction != DOWN)
                    snek.direction = UP;
                break;
            case 'a':
                if (snek.direction != RIGHT)
                    snek.direction = LEFT;
                break;
            case 's':
                if (snek.direction != UP)
                    snek.direction = DOWN;
                break;
            case 'd':
                if (snek.direction != LEFT)
                    snek.direction = RIGHT;
                break;
            default:
                break;
        }
    } while (stepGame(&snek));
    readCharacter(-1);
    closeScreen();
    return 0;
}

bool stepGame(Snek * snek) {
    addNewHead(snek);
    if (isGameOver(snek)) return false;
    LinkedList * snake = snek->snake;
    snake->toStart(snake);
    Point * head = snake->node->data;
    if (head->x == snek->food->x && head->y == snek->food->y) {
        snek->score++;
        placeNewFood(snek);
    } else {
        snake->toEnd(snake);
        snake->removeItem(snake);
    }
    return true;
}

void addNewHead(Snek * snek) {
    LinkedList * snake = snek->snake;
    snake->toStart(snake);
    Point * head = snake->node->data;
    Point * new = malloc(sizeof(Point));
    if (new == NULL) mallocError();
    switch (snek->direction) {
        case UP:
            new->x = head->x;
            new->y = head->y - 1;
            break;
        case DOWN:
            new->x = head->x;
            new->y = head->y + 1;
            break;
        case LEFT:
            new->x = head->x - 1;
            new->y = head->y;
            break;
        case RIGHT:
            new->x = head->x + 1;
            new->y = head->y;
            break;
    }
    if (!snake->addFirst(snake, new)) mallocError();
}

void placeNewFood(Snek * snek) {
    int x, y;
    do {
        x = rand() % (getColumns() - 2) + 1;
        y = rand() % (getRows() - 3) + 2;
    } while (isPointInSnake(snek, x, y, false));
    snek->food->x = x;
    snek->food->y = y;
}

bool isGameOver(const Snek * snek) {
    LinkedList * snake = snek->snake;
    snake->toStart(snake);
    Point * head = snake->node->data;
    if (head->x < 1 || head->y < 2)
        return true;
    if (head->x > getColumns() - 2 || head->y > getRows() - 2)
        return true;
    return isPointInSnake(snek, head->x, head->y, true);
}

bool isPointInSnake(const Snek * snek, int x, int y, bool ignore_head) {
    LinkedList * snake = snek->snake;
    snake->toStart(snake);
    if (ignore_head) {
        if (!snake->hasNext(snake)) return false;
        else snake->next(snake);
    }
    do {
        Point * point = snake->node->data;
        if (point->x == x && point->y == y)
            return true;
    } while (snake->next(snake));
    return false;
}

void mallocError(){
    closeScreen();
    print_error("Couldn't allocate memory\n");
    exit(-3);
}
