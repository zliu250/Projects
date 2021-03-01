#include "bubble.h"

#include "swap.h"

#include <stdbool.h>
#include <stdint.h>

// implementation of bubble sort
void bubble_sort(uint32_t *A, uint32_t n) {
    bool swapped = true;
    uint32_t size = n;
    while (swapped) {
        swapped = false;
        for (uint32_t i = 1; i < size; i++) {
            b_comparison++;
            if (A[i] < A[i - 1]) {
                swaps(&A[i], &A[i - 1]);
                b_moves += 3; // an array swap needs three moves
                swapped = true;
            }
        }
        size--;
    }
    return;
}
