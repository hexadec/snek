//
// Created by hexadec on 11/3/20.
//

#ifndef SNEK_LINKEDLIST_H
#define SNEK_LINKEDLIST_H

#include <stdbool.h>

typedef enum Flags {
    CURRENT = 0x00, BEGIN = 0x01, END = 0x02, EMPTY = 0x04
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
    void (*add)(struct LinkedList *, void * data);
    void (*addFirst)(struct LinkedList *, void * data);
    void (*addLast)(struct LinkedList *, void * data);
    void (*removeItem)(struct LinkedList *);
    void (*seek)(struct LinkedList *, int offset, Flags whence);
    bool (*hasNext)(struct LinkedList *);
    bool (*hasPrevious)(struct LinkedList *);
} LinkedList;

LinkedList * createLinkedList();
void dumpLinkedList(LinkedList *);
static bool next(LinkedList *);
static bool prev(LinkedList *);
static void toStart(LinkedList *);
static void toEnd(LinkedList *);
static void add(LinkedList *, void * data);
static void addFirst(LinkedList *, void * data);
static void addLast(LinkedList *, void * data);
static void removeItem(LinkedList *);
static void seek(LinkedList *, int offset, Flags whence);
static bool hasNext(LinkedList *);
static bool hasPrevious(LinkedList *);

#endif //SNEK_LINKEDLIST_H
