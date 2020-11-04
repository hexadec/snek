//
// Created by hexadec on 11/3/20.
//

#include <stdlib.h>
#include <stdbool.h>
#include "linkedlist.h"

static bool next(LinkedList *);
static bool prev(LinkedList *);
static void toStart(LinkedList *);
static void toEnd(LinkedList *);
static bool add(LinkedList *, void * data);
static bool addFirst(LinkedList *, void * data);
static bool addLast(LinkedList *, void * data);
static void removeItem(LinkedList *);
static void seek(LinkedList *, int offset, Flags whence);
static bool hasNext(LinkedList *);
static bool hasPrevious(LinkedList *);
static size_t size(LinkedList *);

LinkedList * createLinkedList() {
    LinkedList * new = malloc(sizeof (LinkedList));
    new->node = NULL;
    new->next = &next;
    new->prev = &prev;
    new->toStart = &toStart;
    new->toEnd = &toEnd;
    new->add = &add;
    new->addFirst = &addFirst;
    new->addLast = &addLast;
    new->removeItem = &removeItem;
    new->seek = &seek;
    new->hasNext = &hasNext;
    new->hasPrevious = &hasPrevious;
    new->size = &size;
    return new;
}

static bool next(LinkedList * linkedList) {
    if (linkedList != NULL && linkedList->node != NULL && linkedList->node->next != NULL) {
        linkedList->node = linkedList->node->next;
        return true;
    }
    return false;
}

static bool prev(LinkedList * linkedList) {
    if (linkedList != NULL && linkedList->node != NULL && linkedList->node->prev != NULL) {
        linkedList->node = linkedList->node->prev;
        return true;
    }
    return false;
}

static void toStart(LinkedList * linkedList) {
    if (linkedList != NULL && linkedList->node != NULL) {
        while (linkedList->node->prev != NULL)
            linkedList->node = linkedList->node->prev;
    }
}

static void toEnd(LinkedList * linkedList) {
    if (linkedList != NULL && linkedList->node != NULL) {
        while (linkedList->node->next != NULL)
            linkedList->node = linkedList->node->next;
    }
}

static void seek(LinkedList * linkedList, int offset, Flags whence) {
    if (linkedList != NULL && linkedList->node != NULL) {
        int position = 0;
        switch (whence) {
            case BEGIN:
                linkedList->toStart(linkedList);
                while (linkedList->node->next != NULL && position++ < offset)
                    linkedList->node = linkedList->node->next;
                break;
            case END:
                linkedList->toEnd(linkedList);
                while (linkedList->node->prev != NULL && position-- > offset)
                    linkedList->node = linkedList->node->prev;
                break;
            case CURRENT:
                if (offset > 0) {
                    while (linkedList->node->next != NULL && position++ < offset)
                        linkedList->node = linkedList->node->next;
                } else if (offset < 0) {
                    while (linkedList->node->prev != NULL && position++ > offset)
                        linkedList->node = linkedList->node->prev;
                }
                break;
            default:
                break;
        }
    }
}

static bool add(LinkedList * linkedList, void * data) {
    if (linkedList == NULL) return false;
    if (linkedList->node == NULL) {
        linkedList->node = (Node *) malloc(sizeof(Node));
        if (linkedList->node == NULL) return false;
        linkedList->node->data = data;
        linkedList->node->next = NULL;
        linkedList->node->prev = NULL;
    } else {
        Node * new = (Node *) malloc(sizeof(Node));
        if (new == NULL) return false;
        new->data = data;
        new->prev = linkedList->node;
        new->next = linkedList->node->next;
        if (linkedList->node->next != NULL) {
            linkedList->node->next->prev = new;
        }
        linkedList->node->next = new;
        linkedList->node = new;
    }
    return true;
}

static bool addFirst(LinkedList * linkedList, void * data) {
    if (linkedList == NULL) return false;
    linkedList->toStart(linkedList);
    if (linkedList->node == NULL) {
        linkedList->node = (Node *) malloc(sizeof(Node));
        if (linkedList->node == NULL) return false;
        linkedList->node->data = data;
        linkedList->node->next = NULL;
        linkedList->node->prev = NULL;
    } else {
        Node * new = (Node *) malloc(sizeof(Node));
        if (new == NULL) return false;
        new->data = data;
        new->prev = NULL;
        new->next = linkedList->node;
        linkedList->node->prev = new;
        linkedList->node = new;
    }
    return true;
}

static bool addLast(LinkedList * linkedList, void * data) {
    linkedList->toEnd(linkedList);
    return add(linkedList, data);
}

static void removeItem(LinkedList * linkedList) {
    if (linkedList != NULL && linkedList->node != NULL) {
        Node * old = linkedList->node;
        if (old->next == NULL) {
            linkedList->node = old->prev;
            if (linkedList->node != NULL) {
                linkedList->node->next = NULL;
            }
        } else {
            linkedList->node = old->next;
            linkedList->node->prev = old->prev;
        }
        free(old);
    }
}

static bool hasPrevious(LinkedList * linkedList) {
    return linkedList != NULL && linkedList->node != NULL && linkedList->node->prev != NULL;
}

static bool hasNext(LinkedList * linkedList) {
    return linkedList != NULL && linkedList->node != NULL && linkedList->node->next != NULL;
}

static size_t size(LinkedList * linkedList) {
    size_t size = 0;
    if (linkedList == NULL || linkedList->node == NULL) return 0;
    linkedList->toStart(linkedList);
    do {
        size++;
    } while (linkedList->next(linkedList));
    return size;
}

void dumpLinkedList(LinkedList * linkedList) {
    while (linkedList != NULL && linkedList->node != NULL)
        removeItem(linkedList);
    free(linkedList);
}
