#include "quick.h"

#include "stack.h"
#include "swap.h"

#include <inttypes.h>
#include <stdint.h>

//choose the partition for quicksort
int64_t partition(uint32_t A[], int64_t lo, int64_t hi) {
    uint32_t pivot = A[lo + (hi - lo) / 2];
    int64_t i = lo - 1;
    int64_t j = hi + 1;

    do {
        i++;
        while (A[i] < pivot) {
            q_comparison++;
            i++;
        }
        j--;
        q_comparison += 2;
        while (A[j] > pivot) {
            q_comparison++;
            j--;
        }
        if (i < j) {
            swaps(&A[i], &A[j]);
            q_moves += 3;
        }
    } while (i < j);

    return j;
}

// implmentation for quicksort using stack
void quick_sort(uint32_t *A, uint32_t n) {
    int64_t left = 0;
    uint32_t size = n;
    int64_t right = size - 1;

    Stack *s = stack_create();
    stack_push(s, left);
    stack_push(s, right);

    while (stack_empty(s) == false) {
        int64_t hi;
        stack_pop(s, &hi);
        int64_t lo;
        stack_pop(s, &lo);
        int64_t p = partition(A, lo, hi);
        if (p + 1 < hi) {
            stack_push(s, p + 1);
            stack_push(s, hi);
        }
        if (lo < p) {
            stack_push(s, lo);
            stack_push(s, p);
        }
    }
    stack_delete(&s);
}
