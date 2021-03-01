# Math_lib

This program simulates math functions: Sin, Cos, Tan, Exp, and Log when run. These functions are calculated by using taylor approximation while the nth term is smaller than 1e-14

## Build

        $ clang -Wall -Wextra -Werror -Wpedantic -o mathlib-test mathlib-test.o mathlib.o -lm
        
## Running

        $ ./mathlib-test
        
## Cleaning
        
        rm -f mathlib-test *.o
