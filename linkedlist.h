//
// Created by hexadec on 11/3/20.
//

#ifndef SNEK_LINKEDLIST_H
#define SNEK_LINKEDLIST_H

#include <stdbool.h>

typedef enum Flags {
    CURRENT = 0x00, BEGIN = 0x01, END = 0x02
} Flags;

typedef struct Node {
    void * data;
    struct Node * next;
    struct Node * prev;
} Node;

typedef struct LinkedList {
    Node * node;
    bool (*next)(struct LinkedList *);
    bool (*prev)(struct LinkedList *);
    void (*toStart)(struct LinkedList *);
    void (*toEnd)(struct LinkedList *);
    bool (*add)(struct LinkedList *, void * data);
    bool (*addFirst)(struct LinkedList *, void * data);
    bool (*addLast)(struct LinkedList *, void * data);
    void (*removeItem)(struct LinkedList *);
    void (*seek)(struct LinkedList *, int offset, Flags whence);
    bool (*hasNext)(struct LinkedList *);
    bool (*hasPrevious)(struct LinkedList *);
    size_t (*size)(struct LinkedList *);
} LinkedList;

LinkedList * createLinkedList();
void dumpLinkedList(LinkedList *);

#endif //SNEK_LINKEDLIST_H
