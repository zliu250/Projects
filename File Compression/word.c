#include "word.h"

#include "code.h"

#include <memory.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

//constructor for a word
Word *word_create(uint8_t *syms, uint32_t len) {
    Word *w = malloc(sizeof(Word));
    w->len = len;

    w->syms = malloc(sizeof(uint8_t) * len);
    for (uint32_t i = 0; i < len; i++) {
        w->syms[i] = syms[i];
    }
    return w;
}

// add a sym to the word, which means we are creating a new word and extend its size by w->len + 1
Word *word_append_sym(Word *w, uint8_t sym) {
    Word *word = malloc(sizeof(Word));
    if (word) {
        word->len = w->len + 1;
        word->syms = malloc(sizeof(uint8_t) * word->len);
        if (word->syms) {
            memcpy(word->syms, w->syms, w->len);
            word->syms[w->len] = sym;
        }
    }
    return word;
}

// destructor for a word
void word_delete(Word *w) {
    if (w) {
        free(w->syms);
        free(w);
    }
}

// constructor for a word table which is fixed size as MAX_CODE
// only a word at the index EMPTY_CODE in the word table is initialized
WordTable *wt_create(void) {
    WordTable *wt = (WordTable *) calloc(MAX_CODE, sizeof(WordTable));
    if (wt) {
        wt[EMPTY_CODE] = word_create(0, 0);
    }
    return wt;
}

// reset the word table set others to NULL except at EMPTY_CODE position
void wt_reset(WordTable *wt) {
    for (int i = START_CODE; i < MAX_CODE; i++) {
        word_delete(wt[i]);
        wt[i] = NULL;
    }
}

// Destructor for a word table
void wt_delete(WordTable *wt) {
    if (wt) {
        wt_reset(wt);
        word_delete(wt[EMPTY_CODE]);
        free(wt);
        wt = NULL;
    }
}
