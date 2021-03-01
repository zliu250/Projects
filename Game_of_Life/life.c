#include "universe.h"

#include <ncurses.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define OPTIONS "tsn:i:o:"
#define DELAY 50000

int main(int argc, char **argv) {
  FILE *infile = stdin;
  FILE *outfile = stdout;
  int opt = 0;
  int generation = 100;
  bool toroidal = false;
  bool silence = false;

  while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
    switch (opt) {
    case 't':
      toroidal = true;
      break;
    case 's':
      silence = true;
      break;
    case 'n':
      generation = atoi(optarg);
      break;
    case 'i':
      infile = fopen(optarg, "r");
      if (infile == NULL) {
        fprintf(stderr, "Error opening\n");
        exit(0);
      }
      break;
    case 'o':
      outfile = fopen(optarg, "w");
      if (outfile == NULL) {
        fprintf(stderr, "Error outputing\n");
        exit(0);
      }
      break;
    default:
      exit(0);
      break;
    }
  }
  int rows = 0, cols = 0, count = 0;
  ;
  fscanf(infile, "%d %d\n", &rows, &cols);
  Universe *a = uv_create(rows, cols, toroidal); // initialize universe A and B
  Universe *b = uv_create(rows, cols, toroidal);

  // error handling for wrong inputs
  if (uv_populate(a, infile) == false) {
    fprintf(stderr, "Malformed input.");
    exit(0);
  } else {
    for (int i = 0; i < generation; i++) {
      // initialize ncurses screen
      if (silence != true) {
        initscr();
        curs_set(FALSE);
        clear();
        for (int r = 0; r < rows; r++) {
          for (int c = 0; c < cols; c++) {
            if (uv_get_cell(a, r, c) == true) {
              mvprintw(r, c, "o");
            }
          }
        }
        refresh();
        usleep(DELAY);
      }
      // actual implmentation of game of life
      for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
          count += uv_census(a, r, c);
          if (uv_get_cell(a, r, c) == true && (count == 2 || count == 3)) {
            uv_live_cell(b, r, c);
          } else if ((uv_get_cell(a, r, c) == false) && count == 3) {
            uv_live_cell(b, r, c);
          } else {
            uv_dead_cell(b, r, c);
          }
          count = 0;
        }
      }
      Universe *temp = a;
      a = b;
      b = temp;
    }
  }

  // if -s is specified which means we need to close the window
  if (silence != true) {
    endwin();
  }
  uv_print(a, outfile);
  fclose(infile);
  fclose(outfile);
  uv_delete(a);
  uv_delete(b);
  return 0;
}
