#include "code.h"
#include "io.h"
#include "trie.h"

#include <fcntl.h>
#include <getopt.h>
#include <math.h>
#include <memory.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

#define OPTIONS          "vi:o:"
#define bit_length(bits) (floor(log2(bits)) + 1) // macro to get the bit length of given code

int main(int argc, char **argv) {
    char *input = NULL;
    char *output = NULL;
    int infile = STDIN_FILENO;
    int outfile = STDOUT_FILENO;
    int opt = 0;
    bool err_stat = false;
    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'i':
            input = optarg;
            if (input == NULL) {
                fprintf(stderr, "failed to open file");
                exit(1);
            }

            infile = open(input, O_RDONLY); // readonly permission
            if (infile < 0) {
                fprintf(stderr, "open input failed");
                exit(1);
            }
            break;
        case 'o':
            output = optarg;
            if (output == NULL) {
                fprintf(stderr, "failed to open outfile");
                exit(1);
            }
            outfile = open(output, O_WRONLY | O_CREAT | O_TRUNC,
                S_IRWXG | S_IRWXO | S_IRWXU); //permissions: write, overwrite, create
            if (outfile < 0) {
                fprintf(stderr, "open output failed");
                exit(1);
            }
            break;
        case 'v': err_stat = true; break;
        default: fprintf(stderr, "Usage: %s -[vio]\n", argv[0]); break;
        }
    }

    //initialize the file header
    FileHeader header = { 0, 0 };
    header.magic = MAGIC;
    read_header(infile, &header);

    // give permission to outfile
    struct stat inst;
    fstat(infile, &inst);
    header.protection = inst.st_mode;
    fchmod(outfile, header.protection);

    // if the magic number doesn't match, means the file is not compressed with encode.c
    if (header.magic != MAGIC) {
        fprintf(stderr, "Invalid magic number\n");
        close(infile);
        exit(1);
    }

    // initialize the word table for writing out words in outfile
    WordTable *table = wt_create();
    uint8_t curr_sym = 0;
    uint16_t curr_code = 0;
    uint16_t next_code = START_CODE;
    // while there are still remaining pairs to read from infile we read and write them to outfile
    while (read_pair(infile, &curr_code, &curr_sym, bit_length(next_code))) {
        table[next_code] = word_append_sym(table[curr_code], curr_sym);
        write_word(outfile, table[next_code]);
        next_code++;
        if (next_code == MAX_CODE) {
            wt_reset(table);
            next_code = START_CODE;
        }
    }
    flush_words(outfile);

    wt_delete(table);

    // prints out error stats if specified
    if (err_stat) {

        struct stat statsIn;
        struct stat statsOut;

        fstat(infile, &statsIn);
        fstat(outfile, &statsOut);

        printf("Compressed file size: %ld bytes\n", statsIn.st_size);
        printf("Uncompressed file size: %ld bytes\n", statsOut.st_size);
        printf(
            "Space saving: %4.2lf%%\n", 100 * (1 - ((double) statsIn.st_size / statsOut.st_size)));
    }

    close(infile);
    close(outfile);

    return 0;
}
