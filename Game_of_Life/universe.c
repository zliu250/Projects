#include "universe.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

struct Universe {
  int rows;
  int cols;
  bool **grid;
  bool toroidal;
};

// this function only exists in universe.c and check
// for whether or not a cell is in the range of row and col
static bool in_bound(Universe *u, int r, int c) {
  if ((r < u->rows && c < u->cols) && (r >= 0 && c >= 0)) {
    return true;
  } else {
    return false;
  }
}

// this function initialize a universe
Universe *uv_create(int rows, int cols, bool toroidal) {
  Universe *u = (Universe *)calloc(4, sizeof(Universe));
  u->rows = rows;
  u->cols = cols;
  u->toroidal = toroidal;
  u->grid = (bool **)calloc(rows, sizeof(bool *));
  for (int r = 0; r < rows; r++) {
    u->grid[r] = (bool *)calloc(cols, sizeof(bool));
  }
  return u;
}

// this function free up the memory
// allocated to a universe
void uv_delete(Universe *u) {
  for (int r = 0; r < (u->rows); r++) {
    free(u->grid[r]);
  }
  free(u->grid);
  free(u);
  return;
}

// this function return the number of row exists
int uv_rows(Universe *u) { return u->rows; }

// this function return the number of column exists
int uv_cols(Universe *u) { return u->cols; }

// this function turns a cell to alive which means
// true
void uv_live_cell(Universe *u, int r, int c) {
  u->grid[r][c] = true;
  return;
}

// this funciton turns a cell to dead which means
// false
void uv_dead_cell(Universe *u, int r, int c) {
  u->grid[r][c] = false;
  return;
}

// this function return true or false depends
// on if the cell is alive or not
bool uv_get_cell(Universe *u, int r, int c) {
  if (u->toroidal == true) {
    return u
        ->grid[(r + uv_rows(u)) % uv_rows(u)][(c + uv_cols(u)) % uv_cols(u)];
  } else {
    if (in_bound(u, r, c)) {
      return u->grid[r][c];
    } else {
      return false;
    }
  }
}

// this function read the infile and live_cell
// the given coordinates
bool uv_populate(Universe *u, FILE *infile) {
  int r = 0, c = 0;
  while (fscanf(infile, "%d %d\n", &r, &c) != -1) {
    if (in_bound(u, r, c) == true) {
      uv_live_cell(u, r, c);
    } else {
      return false;
    }
  }
  return true;
}

// this function checks for a cell's surrounding
// neighbours and return how many are alive
int uv_census(Universe *u, int r, int c) {
  int alive = 0;
  for (int x = r - 1; x <= (r + 1); x++) {
    for (int y = c - 1; y <= (c + 1); y++) {
      if (uv_get_cell(u, x, y) == true) {
        alive++;
      }
    }
  }
  if (uv_get_cell(u, r, c) == true) {
    alive--;
  }

  return alive;
}

// This uv_print print the final generation into the
// outfile which default as stdout
void uv_print(Universe *u, FILE *outfile) {
  for (int x = 0; x <= (u->rows) - 1; x++) {
    for (int y = 0; y <= (u->cols) - 1; y++) {
      if (uv_get_cell(u, x, y) == true) {
        fputc('o', outfile);
      } else {
        fputc('.', outfile);
      }
    }
    fputc('\n', outfile);
  }
  return;
}
