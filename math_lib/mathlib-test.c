#include "mathlib.h"

#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>

#define OPTIONS "asctel"

// Initialize boolean do_sin,do_cos,do_tan ... as false to make sure
// the program knows which one becomes true
// The if statement and printf statement are formatted to
// print out our approximation function's data
int main(int argc, char **argv) {
    int opt = 0;
    bool do_sin = false, do_cos = false, do_tan = false, do_exp = false, do_log = false;
    while ((opt = getopt(argc, argv, OPTIONS)) != EOF) {
        switch (opt) {
        case 'a':
            do_sin = true;
            do_cos = true;
            do_tan = true;
            do_exp = true;
            do_log = true;
            break;
        case 's': do_sin = true; break;
        case 'c': do_cos = true; break;
        case 't': do_tan = true; break;
        case 'e': do_exp = true; break;
        case 'l': do_log = true; break;
        default: fprintf(stderr, "Usage: %s -[asctel]\n", argv[0]); break;
        }
    }
    if (do_sin) {
        printf("  x            Sin              Library        Difference\n");
        printf("  -            ---              -------        ----------\n");
        for (double x = (-2 * M_PI); x < (2 * M_PI); x += 0.1) {
            printf(" %7.4lf % 16.8lf % 16.8lf % 16.10lf\n", x, Sin(x), sin(x), (Sin(x) - sin(x)));
        }
    }

    if (do_cos) {
        printf("  x            Cos              Library        Difference\n");
        printf("  -            ---              -------        ----------\n");
        for (double x = (-2 * M_PI); x < (2 * M_PI); x += 0.1) {
            printf(" %7.4lf % 16.8lf % 16.8lf % 16.10lf\n", x, Cos(x), cos(x), (Cos(x) - cos(x)));
        }
    }

    if (do_tan) {
        printf("  x            Tan              Library        Difference\n");
        printf("  -            ---              -------        ----------\n");
        for (double x = (-M_PI / 3); x < (M_PI / 3); x += 0.1) {
            printf(" %7.4lf % 16.8lf % 16.8lf % 16.10lf\n", x, Tan(x), tan(x), (Tan(x) - tan(x)));
        }
    }

    if (do_exp) {
        printf("  x            Exp              Library        Difference\n");
        printf("  -            ---              -------        ----------\n");
        for (double x = 1; x < 10; x += 0.1) {
            printf(" %7.4lf % 16.8lf % 16.8lf % 16.10lf\n", x, Exp(x), exp(x), (Exp(x) - exp(x)));
        }
    }

    if (do_log) {
        printf("  x            Log              Library        Difference\n");
        printf("  -            ---              -------        ----------\n");
        for (double x = 1; x < 10; x += 0.1) {
            printf(" %7.4lf % 16.8lf % 16.8lf % 16.10lf\n", x, Log(x), log(x), (Log(x) - log(x)));
        }
    }

    return 0;
}
