#ifndef __NODE_H__
#define __NODE_H__

typedef struct Node Node;

struct Node {
    char *oldspeak;
    char *newspeak;
    Node *next;
    Node *prev;
};

Node *node_create(char *oldspeak, char *newspeak);

void node_delete(Node **n);

void node_print(Node *n);

#endif
