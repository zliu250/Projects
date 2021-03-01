#ifndef __UNIVERSE_H__
#define __UNIVERSE_H__

#include <stdbool.h>
#include <stdio.h>

typedef struct Universe Universe;

Universe *uv_create(int rows, int cols, bool toroidal);

void uv_delete(Universe *u);

int uv_rows(Universe *u);

int uv_cols(Universe *u);

void uv_live_cell(Universe *u, int r, int c);

void uv_dead_cell(Universe *u, int r, int c);

bool uv_get_cell(Universe *u, int r, int c);

bool uv_populate(Universe *u, FILE *infile);

int uv_census(Universe *u, int r, int c);

void uv_print(Universe *u, FILE *outfile);

#endif
