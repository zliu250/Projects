#include "bf.h"
#include "bv.h"
#include "hash.h"
#include "ll.h"
#include "node.h"
#include "parser.h"
#include "speck.h"

#include <ctype.h>
#include <inttypes.h>
#include <limits.h>
#include <regex.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define OPTIONS "h:f:m"
#define BUFFER  1024
#define WORD    "([a-zA-Z0-9])+([-'_][a-zA-Z0-9]+)*" // regex to match words

// this prints the error msg if there are badspeaks
// detected from user inputs
static void error_msg() {
    fprintf(stdout,
        "Dear Comrade,\n\nYou have chosen to use degenerate words that may cause hurt\nfeelings or "
        "cause your comrades to think unpleasant thoughts.\nThis is doubleplus bad. To correct "
        "your wrongthink and\npreserve community consensus we will be sending you to "
        "joycamp\nadministered by Medellin's Miniluv. Beware of the hippos.\n\nYour errors:\n\n");
}

// this prints the joycamp msg if there are oldspeaks
// detected from user inputs
static void old_msg() {
    fprintf(stdout, "Dear Comrade,\n\nSubmitting your text helps to preserve feelings and "
                    "prevent\nbadthink. Some of the words that you used are not goodspeak.\nThe "
                    "list shows how to turn the oldspeak words into newspeak.\n\n");
}

// this function convert the given string
// to lower case
static void to_lower(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = tolower(str[i]);
    }
}

int main(int argc, char **argv) {
    int opt = 0;
    uint32_t h_size = 10000; // default for hash table size
    uint32_t f_size = 1048576; //default bloomfilter size for 2^20
    bool mtf = false;

    //parsing getopt commands
    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case ('h'):
            h_size = (uint32_t) strtoul(optarg, NULL, 10);
            //error handling if h_size is invalid
            if (!h_size) {
                fprintf(stderr, "Invalid hash table size\n");
                exit(1);
            }
            //error handling if hash table size has exceeded
            //the size limits
            if (h_size > INT32_MAX) {
                fprintf(stderr, "Invalid hash table size\n");
                exit(1);
            }
            break;
        case ('f'):
            f_size = (uint32_t) strtoul(optarg, NULL, 10);
            //error handling if f_size is invalid
            if (!f_size) {
                fprintf(stderr, "Invalid bloom filter size\n");
                exit(1);
            }
            //error handling if bloom filter size has exceeded
            //the size limits
            if (f_size > INT32_MAX) {
                fprintf(stderr, "Invalid bloom filter size\n");
                exit(1);
            }
            break;
        case ('m'): mtf = true; break;
        default:
            fprintf(stderr,
                "Usage: %s -[hfm][h hash_table size][f bloom_filter size][m move-to-front rule]\n",
                argv[0]);
            exit(0);
        }
    }

    //initialization of the bloom filter and hash table
    //which will store in badspeak.txt and newspeak.txt
    BloomFilter *bf = bf_create(f_size);
    HashTable *ht = ht_create(h_size, mtf);

    //scan from badspeak.txt and newspeak.txt and insert them into hash table and bloom filter
    //bad_speak serves as a temp to store char read from input file
    //BUFFER make sure that it has enough space to store the given char
    FILE *bads = fopen("badspeak.txt", "r");
    char bad_speak[BUFFER];
    while (fscanf(bads, "%s\n", bad_speak) != -1) {
        bf_insert(bf, bad_speak);
        ht_insert(ht, bad_speak, NULL);
    }
    fclose(bads);

    FILE *news = fopen("newspeak.txt", "r");
    char old_speak[BUFFER];
    char new_speak[BUFFER];
    while (fscanf(news, "%s %s\n", old_speak, new_speak) != -1) {
        bf_insert(bf, old_speak);
        ht_insert(ht, old_speak, new_speak);
    }
    fclose(news);

    //pass stdin into regex to check if the word is valid
    regex_t re;
    if (regcomp(&re, WORD, REG_EXTENDED)) {
        fprintf(stderr, "failed to compile regex");
        return 1;
    }

    //intialize two linked lists which will store
    // comrade's badspeaks and oldspeaks
    // if bloom filter return true, if it also returns a valid node
    // from hash table, that means we detected a badspeak or an oldspeak
    LinkedList *error = ll_create(mtf);
    LinkedList *old = ll_create(mtf);
    char *word = NULL;
    while ((word = next_word(stdin, &re)) != NULL) {
        to_lower(word);
        if (bf_probe(bf, word)) {
            Node *n = ht_lookup(ht, word);
            if (n) {
                (n->newspeak == NULL) ? ll_insert(error, word, NULL)
                                      : ll_insert(old, word, n->newspeak);
            }
        }
    }

    // this part prints out the message depends on the comrade
    // inputted badspeak/oldspeak
    if (ll_length(error) != 0 && ll_length(old) != 0) {
        error_msg();
        ll_print(error);
        printf("\nThink of these words on your vacation!\n\n");
        ll_print(old);
    } else if (ll_length(old) == 0 && ll_length(error) != 0) {
        error_msg();
        ll_print(error);
    } else if (ll_length(error) == 0 && ll_length(old) != 0) {
        old_msg();
        ll_print(old);
    }

    // free allocated memory
    bf_delete(&bf);
    ht_delete(&ht);
    ll_delete(&error);
    ll_delete(&old);
    clear_words();
    regfree(&re);

    return 0;
}
