#include "ll.h"

#include "node.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

struct LinkedList {
    uint32_t length;
    Node *head;
    Node *tail;
    bool mtf;
};

//constructor function of a linked list
LinkedList *ll_create(bool mtf) {
    LinkedList *ll = (LinkedList *) malloc(sizeof(LinkedList));
    ll->length = 0;
    ll->mtf = mtf;
    ll->head = node_create(NULL, NULL);
    ll->tail = node_create(NULL, NULL);
    ll->head->next = ll->tail;
    ll->tail->prev = ll->head;
    return ll;
}

//destructor function of a linked list
void ll_delete(LinkedList **ll) {
    while ((*ll)->head != NULL) {
        Node *temp = (*ll)->head->next;
        node_delete(&(*ll)->head);
        (*ll)->head = temp;
    }
    free(*ll);
    *ll = NULL;
}

//return the length of a linked list
uint32_t ll_length(LinkedList *ll) {
    return ll->length;
}

// look for an oldspeak, if any matches then return that node
// in addition, if move-to-front is specified, move the node
// to the front of the linked list
Node *ll_lookup(LinkedList *ll, char *oldspeak) {
    for (Node *n = ll->head->next; n != ll->tail; n = n->next) {
        if (n->oldspeak) {
            if (strcmp(n->oldspeak, oldspeak) == 0) {
                if (ll->mtf) {
                    n->prev->next = n->next;
                    n->next->prev = n->prev;
                    n->next = ll->head->next;
                    n->prev = ll->head;
                    ll->head->next->prev = n;
                    ll->head->next = n;
                }
                return n;
            }
        }
    }
    return NULL;
}

// this function insert a node into the designated linked list
// while not having a duplicate oldspeak in the linked list
// the node is inserted to the front of the linked list
void ll_insert(LinkedList *ll, char *oldspeak, char *newspeak) {
    if (ll_lookup(ll, oldspeak)) {
        return;
    }
    Node *n = node_create(oldspeak, newspeak);
    n->next = ll->head->next;
    n->prev = ll->head;
    ll->head->next->prev = n;
    ll->head->next = n;
    ll->length++;
    return;
}

//helper function to print out a full linked list
void ll_print(LinkedList *ll) {
    if (ll) {
        for (Node *n = ll->head->next; n != NULL; n = n->next) {
            node_print(n);
        }
    }
}
