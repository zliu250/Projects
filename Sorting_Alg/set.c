#include "set.h"

#include <stdbool.h>
#include <stdint.h>

typedef uint32_t Set;

//initialize a set as 0
Set set_empty(void) {
    return 0;
}

//check if the set is 1 or 0  at position x
bool set_member(Set s, uint8_t x) {
    return s & (1 << x);
}

//change the designated postion x bit in set to 1
Set set_insert(Set s, uint8_t x) {
    return s | (1 << x);
}

// remove the designated postion x bit in set to 0
Set set_remove(Set s, uint8_t x) {
    return s & ~(1 << x);
}

// two sets intersect and return the result
Set set_intersect(Set s, Set t) {
    return s & t;
}

// performs a union of set
Set set_union(Set s, Set t) {
    return s | t;
}

// return the complement
Set set_complement(Set s) {
    return ~s;
}

// return a set that contains the difference of set s and set t
Set set_difference(Set s, Set t) {
    return s & ~t;
}
