#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include "snek.h"
#include "linkedlist.h"

int main() {
    char *test[] = {"aa", "bb", "cc"};
    LinkedList * list = createLinkedList();
    list->toStart(list);
    list->toEnd(list);
    list->add(list, test[2]);
    list->add(list, test[1]);
    list->removeItem(list);
    list->add(list, test[0]);
    //printf("%s", list.node->data);
    list->addFirst(list, "aaaa");
    list->seek(list, 0, END);
    list->add(list, "bbbbb");
    list->toStart(list);
    do {
        printf("%s   ", list->node->data);
    } while (list->next(list));
    printf("\n");
    do {
        printf("%s   ", list->node->data);
    } while (list->prev(list));
    dumpLinkedList(list);
//    /*WINDOW * window =*/ initscr();
//int i = 0; i < 10; i++) {
//        timeout(500);
//        char c = getch();
//        if (c != -1) {
//            printw("%c", c);
//        }
//    }*/
//    for (int i = 0; i < 80; i++) {
//        mvaddch(i % 24, i, '#');
//    }
//    getch();
//    endwin();
    return 0;
}
