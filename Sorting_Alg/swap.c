#include "swap.h"

#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>

//bubblesort comparison/moves
int b_comparison = 0, b_moves = 0;

//shellsort comparison/moves
int sh_comparison = 0, sh_moves = 0;

//heapsort comparison/moves
int h_comparison = 0, h_moves = 0;

//quicksort comparison/moves
int q_comparison = 0, q_moves = 0;

//swap two elements in uint32_t type
void swaps(uint32_t *a, uint32_t *b) {
    uint32_t temp = *a;
    *a = *b;
    *b = temp;
}

// a comparison function to check if a < b
// a and b are array elements and comparison
// is incremented each time this function is called
// regardless if a<b or a>b
bool shell_comparison(uint32_t a, uint32_t b) {
    if (a < b) {
        sh_comparison++;
        return true;
    }
    sh_comparison++;
    return false;
}
