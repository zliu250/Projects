# Sorting algorithms

This program builds four comparison-based sorting algorithms: bubblesort, shellsort, quicksort, and heap sort. The program use a pseudorandom generated array to showcase each sorting. The array has 100 elements default and the elements are within the range of 30bits. The program also counts how many comparisons and moves are made with each sorting algorithm.


## Build

        make all
        
## Running

        $ ./sorting
        
## Cleaning
        
        make clean
        
## format

        make format

## Command Line Option
    
        [-a] enable all sorting [-bsqh] each sort [-r] random seed [-n] size [-p] element
