# Assignment 7

This program performs a lossless LZ78 file compression. By that it means we are taking in any type of infile and encode it by utilizing Trie data structure and array of words. Then by programming an efficient I/O system, we can perform a file compression solely with encode.c and decode.c. Stats such as space saving and compressed/uncompressed bytes can be printed out with command line options.


## Build

        make all, make encode, make decode
        
## Running

        $ ./encode $ ./decode
        
## Cleaning
        
        make clean
        
## format

        make format

## Command Line Option
    
        [-v] specify if you want the error stats [-i] specify input [-o] specify output
