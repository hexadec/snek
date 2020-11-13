//
// Created by hexadec on 11/3/20.
//

#ifndef SNEK_LINKEDLIST_H
#define SNEK_LINKEDLIST_H

#include <stdbool.h>

/**
 * @brief Flags to mark the seek start position
 */
typedef enum Flags {
    CURRENT = 0x00, BEGIN = 0x01, END = 0x02
} Flags;

/**
 * @brief Structure containing a linked list node
 */
typedef struct Node {
    /**
     * Pointer to the held data
     */
    void * data;
    /**
     * Pointer to the next node
     */
    struct Node * next;
    /**
     * Pointer to the previous node
     */
    struct Node * prev;
} Node;

/**
 * @brief Structure containing a linked list
 */
typedef struct LinkedList {
    /**
     * The current node
     */
    Node * node;

    /**
    * Steps the linked list to its next node
    * @param linkedList LinkedList instance to work with
    * @return true on success, false otherwise
    */
    bool (*next)(struct LinkedList *);

    /**
    * Steps the linked list to its previous node
    * @param linkedList LinkedList instance to work with
    * @return true on success, false otherwise
    */
    bool (*prev)(struct LinkedList *);

    /**
    * Steps the linked list to its first node
    * @param linkedList LinkedList instance to work with
    */
    void (*toStart)(struct LinkedList *);

    /**
    * Steps the linked list to its last node
    * @param linkedList LinkedList instance to work with
    */
    void (*toEnd)(struct LinkedList *);

    /**
    * Adds a new node to the linked list after its current position
    * and sets the node to the newly added one
    * @param linkedList LinkedList instance to work with
    * @param data pointer to data to hold
    * @return true on success, false otherwise
    */
    bool (*add)(struct LinkedList *, void * data);

    /**
    * Adds a new node to the start of the linked list
    * and sets the node to the newly added one
    * @param linkedList LinkedList instance to work with
    * @param data pointer to data to hold
    * @return true on success, false otherwise
    */
    bool (*addFirst)(struct LinkedList *, void * data);

    /**
    * Adds a new node to the end of the linked list
    * and sets the node to the newly added one
    * @param linkedList LinkedList instance to work with
    * @param data pointer to data to hold
    * @return true on success, false otherwise
    */
    bool (*addLast)(struct LinkedList *, void * data);

    /**
    * Removed current node from the linked list
    * frees memory pointed by node.data and frees node
    * @param linkedList LinkedList instance to work with
    */
    void (*removeItem)(struct LinkedList *);

    /**
    * Moves the node of the linked list
    * @param linkedList LinkedList instance to work with
    * @param offset number of nodes to move
    * @param whence sets whether to move from current position, start or end
    */
    void (*seek)(struct LinkedList *, int offset, Flags whence);

    /**
    * Checks if the linked list has a next node
    * @param linkedList LinkedList instance to work with
    * @return true if there is a next node, that is not NULL, false otherwise
    */
    bool (*hasNext)(struct LinkedList *);

    /**
    * Checks if the linked list has a previous node
    * @param linkedList LinkedList instance to work with
    * @return true if there is a previous node, that is not NULL, false otherwise
    */
    bool (*hasPrevious)(struct LinkedList *);

    /**
    * Calculates the size of the linked list, and sets the current node to the final position
    * @param linkedList LinkedList instance to work with
    * @return number of nodes in \p linkedList
    */
    size_t (*size)(struct LinkedList *);
} LinkedList;

LinkedList * createLinkedList();
void dumpLinkedList(LinkedList *);

#endif //SNEK_LINKEDLIST_H
