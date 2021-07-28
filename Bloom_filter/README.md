# Bloom filter and Hash table

This program serves as the Great Fire Wall of Santa Cruz. In order to promote virtue and prevent vice, and to preserve social cohesion and discourage unrest, we have decided that the Internet content must be filtered so that the beloved children are not corrupted through the use of unfortunate, hurtful, offensive, and far too descriptive language. In detail, we are applying a bloom filter with the support of hash table of linked lists and filter out these "badspeaks" and "oldspeaks" to purify the environment in Santa Cruz. The bloom filter is composed and accessed by a bitvector.

The program parse inputs from stdin and compile after user enter ctrl-d to end to process of inputting
badspeak.txt and newspeak.txt can be modified for any purposes, this example txt are essentially there for user to modify


## Build

        make all
        
## Running

        $ ./banhammer
        
## Cleaning
        
        make clean
        
## format

        make format

## Command Line Option
    
        [-h] specify the hash table size default 10000 [-f] specify the size of the bloom filter default 2^20 [-m] toggle the move-to-front rule
        

