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

#define OPTIONS "vi:o:"
#define bit_length(bits)                                                                           \
    (floor(log2(bits)) + 1) // this macro return the bit length of the given code

#define byte(bits)                                                                                 \
    ((bits % 8 == 0) ? bits / 8 : bits / 8 + 1) // this macro restricts the bit in range of 8
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
                fprintf(stderr, "input invalid");
                exit(1);
            }

            infile = open(input, O_RDONLY); // give access to the file, read only
            if (infile < 0) {
                fprintf(stderr, "failed to open input");
                exit(1);
            }
            break;
        case 'o':
            output = optarg;
            if (output == NULL) {
                fprintf(stderr, "outfile invalid");
                exit(1);
            }
            outfile = open(output, O_WRONLY | O_CREAT | O_TRUNC,
                S_IRWXG | S_IRWXO
                    | S_IRWXU); // give access to the file, write, create, and overwrite
            if (outfile < 0) {
                fprintf(stderr, "failed to open output");
                exit(1);
            }
            break;
        case 'v': err_stat = true; break;
        default: fprintf(stderr, "Usage: %s -[vio]\n", argv[0]); break;
        }
    }

    // initialize the file header and its magic/protection
    total_bits = 0;
    total_syms = 0;
    FileHeader header = { 0, 0 };
    header.magic = MAGIC;
    struct stat st;
    fstat(infile, &st);
    header.protection = st.st_mode;

    // write out the header to outfile
    write_header(outfile, &header);

    // initialize a trie to store symbol from infile
    TrieNode *root = trie_create();

    TrieNode *curr_node = root;
    TrieNode *prev_node = NULL;
    uint8_t prev_sym = 0;
    uint8_t curr_sym = 0;
    uint16_t next_code = START_CODE;

    // while there are still remaining symbols to read, we fill the trie and write them to outfile
    while (read_sym(infile, &curr_sym)) {
        TrieNode *next_node = trie_step(curr_node, curr_sym);
        if (next_node != NULL) {
            prev_node = curr_node;
            curr_node = next_node;
        } else {
            write_pair(outfile, curr_node->code, curr_sym, bit_length(next_code));
            curr_node->children[curr_sym] = trie_node_create(next_code);
            curr_node = root;
            next_code++;
        }
        if (next_code == MAX_CODE) {
            trie_reset(root);
            curr_node = root;
            next_code = START_CODE;
        }
        prev_sym = curr_sym;
    }
    if (curr_node != root) {
        write_pair(outfile, prev_node->code, prev_sym, bit_length(next_code));
        next_code = (next_code + 1) % MAX_CODE;
    }
    write_pair(outfile, STOP_CODE, 0, bit_length(next_code));

    flush_pairs(outfile);

    trie_delete(root);

    // if error stats are specified to be printed out
    total_bits = byte(total_bits);
    total_syms -= 1;
    struct stat outst;
    fstat(outfile, &outst);
    if (err_stat) {
        float saved = (1 - (double) (total_bits / total_syms)) * 100;
        fprintf(stderr, "Compressed file size: %ld bytes\n", total_bits);
        fprintf(stderr, "Uncompressed file size: %ld bytes\n", total_syms);
        fprintf(stderr, "Space saving: %4.2lf%%\n", saved);
    }

    close(infile);
    close(outfile);

    return 0;
}
