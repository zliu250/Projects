#include "heap.h"

#include "swap.h"

#include <stdbool.h>
#include <stdint.h>

// this function returns the max child for a heap
uint32_t max_child(uint32_t a[], uint32_t first, uint32_t last) {
    uint32_t left = 2 * first;
    uint32_t right = left + 1;

    if (right <= last) {
        if (a[right - 1] > a[left - 1]) {
            h_comparison++;
            return right;
        }
        h_comparison++;
    }

    return left;
}

// this function fix heap if parent is less then its child
void fix_heap(uint32_t a[], uint32_t first, uint32_t last) {
    bool found = false;
    uint32_t parent = first;
    uint32_t great = max_child(a, parent, last);

    while (parent <= last / 2 && found == false) {
        if (a[parent - 1] < a[great - 1]) {
            h_comparison++;
            swaps(&a[parent - 1], &a[great - 1]);
            h_moves += 3;
            parent = great;
            great = max_child(a, parent, last);
        } else {
            h_comparison++;
            found = true;
        }
    }
}

// this function build the heap
void build_heap(uint32_t a[], uint32_t first, uint32_t last) {
    for (uint32_t parent = last / 2; parent > first - 1; parent--) {
        fix_heap(a, parent, last);
    }
}

// this function sort the heap
void heap_sort(uint32_t *A, uint32_t n) {
    uint32_t first = 1;
    uint32_t last = n;
    build_heap(A, first, last);

    for (uint32_t leaf = last; leaf > first; leaf--) {
        swaps(&A[first - 1], &A[leaf - 1]);
        h_moves += 3;
        fix_heap(A, first, leaf - 1);
    }
    return;
}
