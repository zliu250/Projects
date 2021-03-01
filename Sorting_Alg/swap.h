#ifndef __SWAP_H__
#define __SWAP_H__

#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>

int b_comparison, b_moves;

int sh_comparison, sh_moves;

int h_comparison, h_moves;

int q_comparison, q_moves;

void swaps(uint32_t *a, uint32_t *b);

bool shell_comparison(uint32_t a, uint32_t b);

#endif
