#include "trie.h"

#include "code.h"

#include <stdlib.h>

// constructor for a node in a trie
TrieNode *trie_node_create(uint16_t code) {
    TrieNode *n = calloc(1, sizeof(TrieNode));
    if (n) {
        n->code = code;
    }
    return n;
}

// destructor of a node in a trie
void trie_node_delete(TrieNode *n) {
    free(n);
    n = NULL;
}

// constructor for a whole trie which starts at EMPTY_CODE
TrieNode *trie_create(void) {
    return trie_node_create(EMPTY_CODE);
}

// reset trie to just the root, others are set to NULL
void trie_reset(TrieNode *root) {
    if (root) {
        ;
        for (int i = 0; i < ALPHABET; i++) {
            trie_delete(root->children[i]);
            root->children[i] = NULL;
        }
    }
}

// delete the subtrie of pointer n
void trie_delete(TrieNode *n) {
    trie_reset(n);
    trie_node_delete(n);
}

// return a pointer to the symbol from the indexed children
TrieNode *trie_step(TrieNode *n, uint8_t sym) {
    return n->children[sym];
}
