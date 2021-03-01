#include "hash.h"

#include "ll.h"
#include "node.h"
#include "speck.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct HashTable {
    uint64_t salt[2];
    uint32_t size;
    bool mtf;
    LinkedList **lists;
};

//constructor of a hash table
HashTable *ht_create(uint32_t size, bool mtf) {
    HashTable *ht = (HashTable *) malloc(sizeof(HashTable));
    if (ht) {
        ht->salt[0] = 0x85ae998311115ae3;
        ht->salt[1] = 0xb6fac2ae33a40089;
        ht->size = size;
        ht->mtf = mtf;
        ht->lists = (LinkedList **) calloc(size, sizeof(LinkedList *));
        if (!ht->lists) {
            free(ht);
            ht = NULL;
        }
    }
    return ht;
}

//destructor of a hash table
void ht_delete(HashTable **ht) {
    for (uint32_t i = 0; i < ht_size(*ht); i++) {
        if ((*ht)->lists[i]) {
            ll_delete(&(*ht)->lists[i]);
        }
    }
    free((*ht)->lists);
    free(*ht);
    *ht = NULL;
}

// return the size of a hash table
uint32_t ht_size(HashTable *ht) {
    return ht->size;
}

//look for a matching oldspeak in the hash table
//if matched return that found node, else reutnr a NULL pointer
Node *ht_lookup(HashTable *ht, char *oldspeak) {
    uint64_t a = hash(ht->salt, oldspeak) % ht->size;
    if (ht->lists[a]) {
        Node *n = ll_lookup(ht->lists[a], oldspeak);
        return n;
    }
    return NULL;
}

//insert oldspeak and newspeak into the designated place
// by hashing it, additionally if the index returned by hash
// is not initialized in the hash table, it will initialize the empty
// linked list
void ht_insert(HashTable *ht, char *oldspeak, char *newspeak) {
    uint32_t a = hash(ht->salt, oldspeak) % ht->size;

    if (!ht->lists[a]) {
        ht->lists[a] = ll_create(ht->mtf);
    }
    ll_insert(ht->lists[a], oldspeak, newspeak);

    return;
}

// helper function that print out the whole hash table
void ht_print(HashTable *ht) {
    if (ht) {
        for (uint32_t i = 0; i < ht->size; i++) {
            ll_print(ht->lists[i]);
        }
    }
}
