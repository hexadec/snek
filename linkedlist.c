/**
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
 * Steps the linked list to its next node
 * @param linkedList LinkedList instance to work with
 * @return true on success, false otherwise
 */
static bool next(LinkedList * linkedList) {
    if (linkedList != NULL && linkedList->node != NULL && linkedList->node->next != NULL) {
        linkedList->node = linkedList->node->next;
        return true;
    }
    return false;
}

/**
 * Steps the linked list to its previous node
 * @param linkedList LinkedList instance to work with
 * @return true on success, false otherwise
 */
static bool prev(LinkedList * linkedList) {
    if (linkedList != NULL && linkedList->node != NULL && linkedList->node->prev != NULL) {
        linkedList->node = linkedList->node->prev;
        return true;
    }
    return false;
}

/**
 * Steps the linked list to its first node
 * @param linkedList LinkedList instance to work with
 */
static void toStart(LinkedList * linkedList) {
    if (linkedList != NULL && linkedList->node != NULL) {
        while (linkedList->node->prev != NULL)
            linkedList->node = linkedList->node->prev;
    }
}

/**
 * Steps the linked list to its last node
 * @param linkedList LinkedList instance to work with
 */
static void toEnd(LinkedList * linkedList) {
    if (linkedList != NULL && linkedList->node != NULL) {
        while (linkedList->node->next != NULL)
            linkedList->node = linkedList->node->next;
    }
}

/**
 * Moves the node of the linked list
 * @param linkedList LinkedList instance to work with
 * @param offset number of nodes to move
 * @param whence sets whether to move from current position, start or end
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
 * Adds a new node to the linked list after its current position
 * and sets the node to the newly added one
 * @param linkedList LinkedList instance to work with
 * @param data pointer to data to hold
 * @return true on success, false otherwise
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
 * Adds a new node to the start of the linked list
 * and sets the node to the newly added one
 * @param linkedList LinkedList instance to work with
 * @param data pointer to data to hold
 * @return true on success, false otherwise
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
 * Adds a new node to the end of the linked list
 * and sets the node to the newly added one
 * @param linkedList LinkedList instance to work with
 * @param data pointer to data to hold
 * @return true on success, false otherwise
 */
static bool addLast(LinkedList * linkedList, void * data) {
    linkedList->toEnd(linkedList);
    return add(linkedList, data);
}

/**
 * Removed current node from the linked list
 * frees memory pointed by node.data and frees node
 * @param linkedList LinkedList instance to work with
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
 * Checks if the linked list has a previous node
 * @param linkedList LinkedList instance to work with
 * @return true if there is a previous node, that is not NULL, false otherwise
 */
static bool hasPrevious(LinkedList * linkedList) {
    return linkedList != NULL && linkedList->node != NULL && linkedList->node->prev != NULL;
}

/**
 * Checks if the linked list has a next node
 * @param linkedList LinkedList instance to work with
 * @return true if there is a next node, that is not NULL, false otherwise
 */
static bool hasNext(LinkedList * linkedList) {
    return linkedList != NULL && linkedList->node != NULL && linkedList->node->next != NULL;
}

/**
 * Calculates the size of the linked list, and sets the current node to the final position
 * @param linkedList LinkedList instance to work with
 * @return number of nodes in \p linkedList
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
