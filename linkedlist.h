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
    /** @brief Seek from current position */
    CURRENT = 0x00,
    /** @brief Seek from the beginning of the list */
    BEGIN = 0x01,
    /** @brief Seek from the end of the list */
    END = 0x02
} Flags;

/**
 * @brief Structure containing a linked list node
 */
typedef struct Node {
    /**
     * Pointer to the contained data. Needs to be dynamically allocated.
     * @brief Pointer to the held data
     */
    void * data;
    /**
     * Pointer to the next node of the list.
     * NULL if it's the last node.
     * @brief Pointer to the next node
     */
    struct Node * next;
    /**
     * Pointer to the previous node of the list.
     * NULL if it's the first node.
     * @brief Pointer to the previous node
     */
    struct Node * prev;
} Node;

/**
 * This is a structure that holds a linked list, and all related methods are accessible from here.
 * Function pointers are included, in order to avoid spamming the project with function names,
 * that would otherwise confuse the developer and possibly conflict with other functions.
 * Please note, that this is a general-purpose linked list,
 * therefore many functions are unused in this project.
 * @brief Structure containing a general-purpose linked list
 */
typedef struct LinkedList {
    /**
     * @brief The current node
     */
    Node * node;

    /**
    * Sets the node of the linked list to the next position, if there is one.
    * @brief Steps the linked list to its next node
    * @param linkedList LinkedList instance to work with
    * @return true on success, false otherwise
    */
    bool (*next)(struct LinkedList *);

    /**
    * Sets the node of the linked list to the next position, if there is one.
    * @brief Steps the linked list to its previous node
    * @param linkedList LinkedList instance to work with
    * @return true on success, false otherwise
    */
    bool (*prev)(struct LinkedList *);

    /**
    * Sets the node of the linked list to the starting position.
    * Nothing happens if the list is empty.
    * @brief Steps the linked list to its first node
    * @param linkedList LinkedList instance to work with
    */
    void (*toStart)(struct LinkedList *);

    /**
    * Sets the node of the linked list to the end position.
    * Nothing happens if the list is empty.
    * @brief Steps the linked list to its last node
    * @param linkedList LinkedList instance to work with
    */
    void (*toEnd)(struct LinkedList *);

    /**
    * Inserts a new node at the linked list after the current node,
    * and sets the current node to the newly added one
    * @brief Adds a new node to the linked list after its current position
    * @param linkedList LinkedList instance to work with
    * @param data pointer to data to hold
    * @return true on success, false otherwise
    * @attention \p data needs to be dynamically allocated, as calling \p removeItem frees its memory
    */
    bool (*add)(struct LinkedList *, void * data);

    /**
    * Inserts a new node at the start of the linked list
    * and sets the current node to the newly added one
    * @brief Adds a new node to the start of the linked list
    * @param linkedList LinkedList instance to work with
    * @param data pointer to data to hold
    * @return true on success, false otherwise
    * @attention \p data needs to be dynamically allocated, as calling \p removeItem frees its memory
    */
    bool (*addFirst)(struct LinkedList *, void * data);

    /**
    * Appends a new node at the end of the linked list
    * and sets the current node to the newly added one
    * @brief Adds a new node to the start of the linked list
    * @param linkedList LinkedList instance to work with
    * @param data pointer to data to hold
    * @return true on success, false otherwise
    * @attention \p data needs to be dynamically allocated, as calling \p removeItem frees its memory
    */
    bool (*addLast)(struct LinkedList *, void * data);

    /**
    * Removed current node from the linked list,
    * also frees memory pointed by node.data and frees node,
    * therefore only dynamically allocated data can be inserted
    * @brief Removes current node from the linked list
    * @param linkedList LinkedList instance to work with
    * @attention \p data needs to be dynamically allocated
    */
    void (*removeItem)(struct LinkedList *);

    /**
    * Moves the node of the linked list by a given offset, in a given direction.
    * Over-indexing will not cause any errors, the node will set to the first/last one.
    * @brief Moves the node of the linked list
    * @param linkedList LinkedList instance to work with
    * @param offset number of nodes to move
    * @param whence sets whether to move from current position, start or end
    */
    void (*seek)(struct LinkedList *, int offset, Flags whence);

    /**
    * @brief Checks if the linked list has a next node
    * @param linkedList LinkedList instance to work with
    * @return true if there is a next node, that is not NULL, false otherwise
    */
    bool (*hasNext)(struct LinkedList *);

    /**
    * @brief Checks if the linked list has a previous node
    * @param linkedList LinkedList instance to work with
    * @return true if there is a previous node, that is not NULL, false otherwise
    */
    bool (*hasPrevious)(struct LinkedList *);

    /**
    * Returns the size of the linked list by iterating through all elements.
    * This practice is necessary to avoid any accidental modifications
    * of a size parameter in the structure. An important side-effect is, that the node
    * will point to the last element.
    * @brief Calculates the size of the linked list
    * @param linkedList LinkedList instance to work with
    * @return number of nodes in \p linkedList
    */
    size_t (*size)(struct LinkedList *);

    /**
     * Swaps the data of two nodes in the linked list.
     * The current node will point to the beginning afterwards.
     * @brief Swaps the data of two nodes in the linked list.
     * @param linkedList LinkedList instance to work with
     * @param index1 index of first node from the beginning of the list
     * @param index2 index of second node from the beginning of the list
     */
    void (*swap)(struct LinkedList *, int, int);
} LinkedList;

/**
 * @brief Creates a \p LinkedList instance
 * @return a \p LinkedList instance
 */
LinkedList * createLinkedList();

/**
 * Frees all memory used by \p linkedList
 * Uses removeItem to remove all items and then frees the memory used by the instance as well
 * @brief Frees all memory used by the \p LinkedList instance
 * @param linkedList LinkedList instance to work with
 */
void dumpLinkedList(LinkedList *);

#endif //SNEK_LINKEDLIST_H
