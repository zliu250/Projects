#include "stack.h"

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>

struct Stack {
    uint32_t top;
    uint32_t capacity;
    int64_t *items;
};

//initialize stack using calloc to allocate memory
Stack *stack_create(void) {
    Stack *s = (Stack *) calloc(1, sizeof(Stack));
    s->top = 0;
    s->capacity = MIN_CAPACITY;
    s->items = (int64_t *) calloc(MIN_CAPACITY, sizeof(int64_t));
    return s;
}

// free up memory allocated to a stack
void stack_delete(Stack **s) {
    free((*s)->items);
    free(*s);
}

// check if a stack is empty or not
bool stack_empty(Stack *s) {
    return s->top == 0;
}

// push in a number into the stack, return null if fail to reallocate memory
bool stack_push(Stack *s, int64_t x) {
    if (s->top == s->capacity) {
        s->capacity *= 2;
        s->items = (int64_t *) realloc(s->items, s->capacity * sizeof(int64_t));
        if (s->items == NULL) {
            return false;
        }
    }
    s->items[s->top] = x;
    s->top += 1;
    return true;
}

// pop the top element in stack to pointer x
bool stack_pop(Stack *s, int64_t *x) {
    if (!stack_empty(s)) {
        s->top -= 1;
        *x = s->items[s->top];
        return true;
    }
    return false;
}

// print out stack,  helper function
void stack_print(Stack *s) {
    for (uint32_t x = 0; x < s->capacity; x++) {
        printf("%" PRId64 ", ", s->items[x]);
    }
}
