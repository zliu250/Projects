#ifndef __BF_H__
#define __BF_H__

#include "bv.h"

#include <stdbool.h>
#include <stdint.h>

typedef struct BloomFilter BloomFilter;

BloomFilter *bf_create(uint32_t size);

void bf_delete(BloomFilter **bf);

uint32_t bf_length(BloomFilter *bf);

void bf_insert(BloomFilter *bf, char *oldspeak);

bool bf_probe(BloomFilter *bf, char *oldspeak);

void bf_print(BloomFilter *bf);

#endif
