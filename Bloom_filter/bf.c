#include "bf.h"

#include "bv.h"
#include "hash.h"
#include "ll.h"
#include "node.h"
#include "speck.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

struct BloomFilter {
    uint64_t primary[2];
    uint64_t secondary[2];
    uint64_t tertiary[2];
    BitVector *filter;
};

//constructor of bloom filter
BloomFilter *bf_create(uint32_t size) {

    BloomFilter *bf = (BloomFilter *) malloc(sizeof(BloomFilter));
    if (bf) {
        bf->primary[0] = 0x02d232593fbe42ff;
        bf->primary[1] = 0x3775cfbf0794f152;

        bf->secondary[0] = 0xc1706bc17ececc04;
        bf->secondary[1] = 0xe9820aa4d2b8261a;

        bf->tertiary[0] = 0xd37b01df0ae8f8d0;
        bf->tertiary[1] = 0x911d454886ca7cf7;

        bf->filter = bv_create(size);
        if (!bf->filter) {
            free(bf);
            bf = NULL;
        }
    }
    return bf;
}

//destructor of bloom filter
void bf_delete(BloomFilter **bf) {
    bv_delete(&(*bf)->filter);
    free(*bf);
    *bf = NULL;
}

// return the size of the bloom filter
uint32_t bf_length(BloomFilter *bf) {
    return bv_length(bf->filter);
}

// this function toggles the bitvector using bv_set_bit
// index is determined by hashing oldspeak and mod by the
// bloom filter size
void bf_insert(BloomFilter *bf, char *oldspeak) {
    uint32_t a = hash(bf->primary, oldspeak) % bv_length(bf->filter);
    uint32_t b = hash(bf->secondary, oldspeak) % bv_length(bf->filter);
    uint32_t c = hash(bf->tertiary, oldspeak) % bv_length(bf->filter);

    bv_set_bit(bf->filter, a);
    bv_set_bit(bf->filter, b);
    bv_set_bit(bf->filter, c);
}

// this function true or false depends on the bloom filter reports
// a false positive(bv_get_bit returns 1 for all three places)
// or a definitely negative(not all of them return a 1).
bool bf_probe(BloomFilter *bf, char *oldspeak) {
    uint32_t a = hash(bf->primary, oldspeak) % bv_length(bf->filter);
    uint32_t b = hash(bf->secondary, oldspeak) % bv_length(bf->filter);
    uint32_t c = hash(bf->tertiary, oldspeak) % bv_length(bf->filter);

    uint32_t e = bv_get_bit(bf->filter, a);
    uint32_t f = bv_get_bit(bf->filter, b);
    uint32_t g = bv_get_bit(bf->filter, c);

    return ((e + f + g) == 3) ? true : false;
}

//helper function that prints out the bloom filter
// which is a bit vecotr consists of 1s and 0s
void bf_print(BloomFilter *bf) {
    bv_print(bf->filter);
}
