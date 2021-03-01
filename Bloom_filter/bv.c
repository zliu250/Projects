#include "bv.h"

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>

// this function return the column index in the range of 0-7
static uint32_t bit_to_byte(uint32_t x) {
    return ((x % 8 == 0) ? x / 8 : (x / 8 + 1));
}

struct BitVector {
    uint32_t length;
    uint8_t *vector;
};

// this function creates a bit vector
BitVector *bv_create(uint32_t length) {
    BitVector *bv = (BitVector *) calloc(length, sizeof(BitVector));
    if (bv) {
        bv->length = length;
        bv->vector = (uint8_t *) calloc(bit_to_byte(length), sizeof(uint8_t));
        if (bv->vector) {
            return bv;
        } else {
            free(bv);
            bv = NULL;
        }
    }
    return NULL;
}

// destructor of the bitvector
void bv_delete(BitVector **bv) {
    free((*bv)->vector);
    free(*bv);
    *bv = NULL;
}

//return the size of the bit vector
uint32_t bv_length(BitVector *bv) {
    return bv->length;
}

//toggle the [i] bit to 1 from the bit vector
void bv_set_bit(BitVector *bv, uint32_t i) {
    uint32_t index = i % 8;
    uint32_t mask = 1 << index;
    bv->vector[index] |= mask;
    return;
}

//clear the [i] bit to 0 from the bit vector
void bv_clr_bit(BitVector *bv, uint32_t i) {
    uint32_t index = i % 8;
    uint32_t mask = ~(1 << index);
    bv->vector[index] &= mask;
    return;
}

//get the [i] bit in the bitvecotr, return 1 or 0
uint8_t bv_get_bit(BitVector *bv, uint32_t i) {
    uint32_t index = i % 8;
    uint32_t mask = 1 << index;
    return (bv->vector[index] &= mask) >> index;
}

void bv_print(BitVector *bv) {
    printf("%d", bv->length);
    return;
}
