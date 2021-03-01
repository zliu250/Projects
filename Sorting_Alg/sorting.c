#include "bubble.h"
#include "heap.h"
#include "quick.h"
#include "set.h"
#include "shell.h"
#include "stack.h"
#include "swap.h"

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define OPTIONS "absqhr:n:p:"
#define MASK    ~(3 << 30) // max value of a 30-bit-number

enum sorting { bubble = 0, shell = 1, quick = 2, heap = 3 };

// fill up an array
void fill(uint32_t b[], uint32_t n, uint32_t seed) {
    srand(seed);
    for (uint32_t i = 0; i < n; i++) {
        b[i] = (rand() & MASK);
    }
}

int main(int argc, char **argv) {
    uint32_t n = 100; // array size
    uint32_t p = 100;
    Set sorts = set_empty();
    int opt = 0;
    int seed = 7092016;
    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case ('a'):
            for (int i = bubble; i < heap + 1; i++) {
                sorts = set_insert(sorts, i);
            }
            break;
        case ('b'): sorts = set_insert(sorts, bubble); break;
        case ('s'): sorts = set_insert(sorts, shell); break;
        case ('q'): sorts = set_insert(sorts, quick); break;
        case ('h'): sorts = set_insert(sorts, heap); break;
        case ('r'): seed = atoi(optarg); break;
        case ('n'): n = atoi(optarg); break;
        case ('p'): p = atoi(optarg); break;
        default:
            fprintf(stderr, "Usage: %s -[absqh][r seed][n array size][p elements]\n", argv[0]);
            exit(0);
        }
    }

    uint32_t counter = (p < n) ? p : n; // how many elements to print
    for (int i = bubble; i < heap + 1; i++) {
        if (set_member(sorts, i)) {
            uint32_t *b = (uint32_t *) malloc(n * sizeof(uint32_t));
            fill(b, n, seed);
            switch (i) {
            case (0):
                bubble_sort(b, n);
                printf("Bubble Sort\n%" PRIu32 " elements, %d moves, %d compares\n", n, b_moves,
                    b_comparison);
                for (uint32_t i = 0; i < counter; i++) {
                    printf("%13" PRIu32, b[i]);
                    if ((i + 1) % 5 == 0 && i != 0) {
                        printf("\n");
                    }
                }
                break;
            case (1):
                shell_sort(b, n);
                printf("Shell Sort\n%" PRIu32 " elements, %d moves, %d compares\n", n, sh_moves,
                    sh_comparison);
                for (uint32_t i = 0; i < counter; i++) {
                    printf("%13" PRIu32, b[i]);
                    if ((i + 1) % 5 == 0 && i != 0) {
                        printf("\n");
                    }
                }
                break;
            case (2):
                quick_sort(b, n);
                printf("Quick Sort\n%" PRIu32 " elements, %d moves, %d compares\n", n, q_moves,
                    q_comparison);
                for (uint32_t i = 0; i < counter; i++) {
                    printf("%13" PRIu32, b[i]);
                    if ((i + 1) % 5 == 0 && i != 0) {
                        printf("\n");
                    }
                }
                break;
            case (3):
                heap_sort(b, n);
                printf("Heap Sort\n%" PRIu32 " elements, %d moves, %d compares\n", n, h_moves,
                    h_comparison);
                for (uint32_t i = 0; i < counter; i++) {
                    printf("%13" PRIu32, b[i]);
                    if ((i + 1) % 5 == 0 && i != 0) {
                        printf("\n");
                    }
                }
                break;
            }
            free(b);
        }
    }
}
