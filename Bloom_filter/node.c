#include "node.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//mimic function of stdup
// takes in a char pointer and copy the contents of that char
#define strup(s) strcpy(malloc(strlen(s) + 1), s)

//constructor function of node and takes a copy of oldspeak and newspeak
Node *node_create(char *oldspeak, char *newspeak) {
    Node *n = (Node *) malloc(sizeof(Node));
    n->oldspeak = (oldspeak == NULL) ? NULL : strup(oldspeak);
    n->newspeak = (newspeak == NULL) ? NULL : strup(newspeak);
    n->next = NULL;
    n->prev = NULL;
    return n;
}

//destructor function of a node
void node_delete(Node **n) {
    free((*n)->oldspeak);
    free((*n)->newspeak);
    free(*n);
    *n = NULL;
    return;
}

//helper function, formatted to print out badspeak or oldspeak -> translation of it
void node_print(Node *n) {
    if (n && n->oldspeak && !n->newspeak) {
        printf("%s\n", n->oldspeak);
    }
    if (n && n->oldspeak && n->newspeak) {
        printf("%s -> %s\n", n->oldspeak, n->newspeak);
    }
}
