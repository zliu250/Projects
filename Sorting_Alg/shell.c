#include "shell.h"

#include "gaps.h"
#include "swap.h"

#include <stdint.h>

// implementation of shell sort
void shell_sort(uint32_t *A, uint32_t n) {
    for (uint32_t i = 0; i < GAPS; i++) {
        for (uint32_t x = gaps[i]; x < n; x++) {
            uint32_t j = x;
            uint32_t temp = A[x];
            while (j >= gaps[i] && shell_comparison(temp, A[j - gaps[i]])) {
                sh_moves += 3;
                swaps(&A[j], &A[j - gaps[i]]);
                j -= gaps[i];
            }
            A[j] = temp;
        }
    }
}
