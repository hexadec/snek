/**
 * This file contains the functions that are required to work with linked lists.
 * Most of these functions are static and only accessible through a function pointer
 * from a \p LinkedList instance. This is done in order to prevent unnecessary
 * function definitions in other files.
 * \attention LinkedList works with dynamically allocated data, as it frees memory
 * whenever an action occurs that makes a pointer inaccessible for later use.
 * \file linkedlist.c
 * \author hexadec
 * \brief This file contains the logic required to work with linked lists
 */

#include <stdlib.h>
#include <stdbool.h>
#include "linkedlist.h"
#include "debugmalloc.h"

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

/**
 * Creates a LinkedList instance
 * @return a LinkedList instance
 */
LinkedList * createLinkedList() {
    LinkedList * new = malloc(sizeof (LinkedList));
    if (new == NULL) return NULL;
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

/**
 * @copydoc LinkedList.next
 */
static bool next(LinkedList * linkedList) {
    if (linkedList != NULL && linkedList->node != NULL && linkedList->node->next != NULL) {
        linkedList->node = linkedList->node->next;
        return true;
    }
    return false;
}

/**
 * @copydoc LinkedList.prev
 */
static bool prev(LinkedList * linkedList) {
    if (linkedList != NULL && linkedList->node != NULL && linkedList->node->prev != NULL) {
        linkedList->node = linkedList->node->prev;
        return true;
    }
    return false;
}

/**
 * @copydoc LinkedList.toStart
 */
static void toStart(LinkedList * linkedList) {
    if (linkedList != NULL && linkedList->node != NULL) {
        while (linkedList->node->prev != NULL)
            linkedList->node = linkedList->node->prev;
    }
}

/**
 * @copydoc LinkedList.toEnd
 */
static void toEnd(LinkedList * linkedList) {
    if (linkedList != NULL && linkedList->node != NULL) {
        while (linkedList->node->next != NULL)
            linkedList->node = linkedList->node->next;
    }
}

/**
 * @copydoc LinkedList.seek
 */
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

/**
 * @copydoc LinkedList.add
 */
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

/**
 * @copydoc LinkedList.addFirst
 */
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

/**
 * @copydoc LinkedList.addLast
 */
static bool addLast(LinkedList * linkedList, void * data) {
    linkedList->toEnd(linkedList);
    return add(linkedList, data);
}

/**
 * @copydoc LinkedList.removeItem
 */
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
        free(old->data);
        free(old);
    }
}

/**
 * @copydoc LinkedList.hasPrevious
 */
static bool hasPrevious(LinkedList * linkedList) {
    return linkedList != NULL && linkedList->node != NULL && linkedList->node->prev != NULL;
}

/**
 * @copydoc LinkedList.hasNext
 */
static bool hasNext(LinkedList * linkedList) {
    return linkedList != NULL && linkedList->node != NULL && linkedList->node->next != NULL;
}

/**
 * @copydoc LinkedList.size
 */
static size_t size(LinkedList * linkedList) {
    size_t size = 0;
    if (linkedList == NULL || linkedList->node == NULL) return 0;
    linkedList->toStart(linkedList);
    do {
        size++;
    } while (linkedList->next(linkedList));
    return size;
}

/**
 * Frees all memory used by \p linkedList
 * Uses removeItem to remove all items and then frees the memory used by the instance as well
 * @param linkedList LinkedList instance to work with
 */
void dumpLinkedList(LinkedList * linkedList) {
    while (linkedList != NULL && linkedList->node != NULL)
        removeItem(linkedList);
    free(linkedList);
}
