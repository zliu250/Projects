#include "io.h"

#include "code.h"
#include "endian.h"

#include <memory.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

static uint8_t symbol[BLOCK] = { 0 };
static uint8_t bit_buffer[BLOCK] = { 0 };
static int sym_idx = 0;
static int bit_idx = 0;

static int end = -1;

uint64_t total_syms;
uint64_t total_bits;

#define byte(bits)                                                                                 \
    ((bits % 8 == 0) ? bits / 8 : bits / 8 + 1) // this macro restricts the bit in range of 8

// wrapper function for read() which reads out how many bytes from infile
int read_bytes(int infile, uint8_t *buf, int to_read) {
    int num_byte = 0;
    while (num_byte < to_read) {
        int bytes = read(infile, &buf[num_byte], to_read - num_byte);
        if (bytes <= 0) {
            break;
        }
        num_byte += bytes;
    }

    return num_byte;
}

// similar to write bytes, wrapper function, return how many bytes from outfile
int write_bytes(int outfile, uint8_t *buf, int to_write) {
    int num_byte = 0;
    while (num_byte < to_write) {
        int size = to_write - num_byte;
        int bytes = write(outfile, &buf[num_byte], size);
        if (bytes <= 0) {
            break;
        }
        num_byte += bytes;
    }

    return num_byte;
}

// read the file descriptor's header and check its magic/protection
void read_header(int infile, FileHeader *header) {
    if (big_endian()) {
        header->magic = swap32(header->magic);
        header->protection = swap16(header->protection);
    }
    total_bits += 8 * sizeof(header);

    read_bytes(infile, (uint8_t *) header, sizeof(FileHeader));
    return;
}

// write the fileheader to outfile
void write_header(int outfile, FileHeader *header) {
    total_bits += 8 * sizeof(header);
    write_bytes(outfile, (uint8_t *) header, sizeof(FileHeader));
    return;
}

// if no symbols need to be read return false else return true
// inspired by Sahiti's section, end is used as a counter to check if end needs to be updated
// return true if end was not updated which stays at -1
bool read_sym(int infile, uint8_t *sym) {
    total_syms += 1;
    if (sym_idx == 0) {
        int bytes = read_bytes(infile, symbol, BLOCK);
        if (bytes < BLOCK) {
            end = bytes + 1; // update the end of the buffer if less than a block is read
        }
    }
    *sym = symbol[sym_idx];
    sym_idx++;
    if (sym_idx == BLOCK) {
        sym_idx = 0;
    }
    return sym_idx != end;
}

// inspired by Eugene's section, especially the bitwise operation
// puts a code and a symbol together as a pair store that to the bit buffer in order to write it to outfile
void write_pair(int outfile, uint16_t code, uint8_t sym, int bitlen) {
    total_bits += 8 + bitlen;
    if (big_endian()) {
        code = swap16(code);
    }

    for (int i = 0; i < bitlen; i++) {
        bool bit = (code >> i) & 1;
        if (bit) {
            bit_buffer[bit_idx / 8] |= (1 << (bit_idx % 8));
        } else {
            bit_buffer[bit_idx / 8] &= ~(1 << (bit_idx % 8));
        }
        bit_idx++;
        if (bit_idx >= BLOCK * 8) {
            write_bytes(outfile, bit_buffer, BLOCK);
            bit_idx = 0;
        }
    }

    for (int i = 0; i < 8; i++) {
        bool bit = (sym >> i) & 1;
        if (bit) {
            bit_buffer[bit_idx / 8] |= (1 << (bit_idx % 8));
        } else {
            bit_buffer[bit_idx / 8] &= ~(1 << (bit_idx % 8));
        }
        bit_idx++;
        if (bit_idx >= BLOCK * 8) {
            write_bytes(outfile, bit_buffer, BLOCK);
            bit_idx = 0;
        }
    }

    return;
}

// write out any remaining pairs
void flush_pairs(int outfile) {
    if (bit_idx != 0) {
        write_bytes(outfile, bit_buffer, byte(bit_idx));
    }
    memset(bit_buffer, 0, BLOCK);
    bit_idx = 0;
}

// read the buffered pair from infile and seperate them back into symbol and code
// basically a copy pasta from Eugene's section
bool read_pair(int infile, uint16_t *code, uint8_t *sym, int bitlen) {
    total_bits += 8 + bitlen;
    uint16_t tempcode = 0;
    for (int i = 0; i < bitlen; i++) {
        if (bit_idx == 0) {
            read_bytes(infile, bit_buffer, BLOCK);
        }
        bool bit = (bit_buffer[bit_idx / 8] & (1 << bit_idx % 8)) >> (bit_idx % 8);
        if (bit) {
            tempcode |= (1 << (i % 16)); // i % 16 is to make sure the range is within uin16_t
        } else {
            tempcode &= ~(1 << (i % 16));
        }
        bit_idx++;
        if (bit_idx == BLOCK * 8) {
            bit_idx = 0;
        }
    }

    *code = tempcode;
    if (big_endian()) {
        *code = swap16(tempcode);
    }

    uint8_t tempsym = 0;
    for (int i = 0; i < 8; i++) {
        if (bit_idx == 0) {
            read_bytes(infile, bit_buffer, BLOCK);
        }
        bool bit = (bit_buffer[bit_idx / 8] & (1 << bit_idx % 8)) >> (bit_idx % 8);
        if (bit) {
            tempsym |= (1 << (i % 8));
        } else {
            tempsym &= ~(1 << (i % 8));
        }
        bit_idx++;
        if (bit_idx == BLOCK * 8) {
            bit_idx = 0;
        }
    }

    if (*code != STOP_CODE) {
        *sym = 0;
        *sym = tempsym;
        return true;
    } else {
        return false;
    }
}

// write the word to outfile, each word is placed into a symbol buffer
void write_word(int outfile, Word *w) {
    for (uint32_t i = 0; i < w->len; i++) {
        symbol[sym_idx] = w->syms[i];
        sym_idx++;
        if (sym_idx >= BLOCK) {
            write_bytes(outfile, symbol, BLOCK);
            sym_idx = 0;
        }
    }
}

// writes out any remaining words to the symbol buffer
void flush_words(int outfile) {
    if (sym_idx != 0) {
        write_bytes(outfile, symbol, sym_idx);
    }
    sym_idx = 0;
}
