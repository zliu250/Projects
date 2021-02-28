#include "names.h"

#include <inttypes.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

void start();
int left(int pos, int total_player);
int right(int pos, int total_player);

int main(void) {
    int player_count;
    int64_t random_seed;

    printf("Number of players: ");
    if ((scanf("%d", &player_count) == 0) || (player_count < 2 || player_count > 10)) {
        fprintf(stderr, "Invalid number of players.\n");
        return 1;
    }

    printf("Random seed: ");
    if ((scanf("%" SCNd64, &random_seed) != 1) || (random_seed < 0 || random_seed > UINT32_MAX)) {
        fprintf(stderr, "Invalid random seed.\n");
        return 1;
    }

    srand(random_seed);
    start(player_count);
    return 0;
}

// The start function is mainly structured with a while loop
// which will simulate the Garlic game while the lone winner
// has not been selected.
//
// Inputs are entered by users before the function operates
// The function will generate the game's output in text after
// valid inputs are captured.
void start(int player_count) {
    int player_lives[player_count], round_counter = 1, total_player = player_count;

    for (int i = 0; i < player_count; i++) {
        player_lives[i] = 3;
    }

    while (1) {
        int lowest_num = 11, lowest_idx = 0, death_counter = 0;

        printf("Round %d\n", round_counter);

        for (int i = 0; i < total_player; i++) {
            if (player_lives[i] != 0) {
                int first = (rand() % 6);
                int second = (rand() % 6);
                if ((first + second) == 10) {
                    printf(" - %s rolls %s... ", names[i], rolls[first][second]);
                    if ((player_lives[left(i, total_player)]) == 0) {
                        player_lives[left(i, total_player)] += 1;
                        printf("resurrects %s ", names[left(i, total_player)]);
                    } else {
                        player_lives[left(i, total_player)] += 1;
                        printf("sparkles %s ", names[left(i, total_player)]);
                    }
                    if ((player_lives[right(i, total_player)]) == 0) {
                        player_lives[right(i, total_player)] += 1;
                        printf("resurrects %s\n", names[right(i, total_player)]);
                    } else {
                        player_lives[right(i, total_player)] += 1;
                        printf("sparkles %s\n", names[right(i, total_player)]);
                    }
                } else {
                    printf(" - %s rolls %s...\n", names[i], rolls[first][second]);
                }
                if ((first + second) < lowest_num) {
                    lowest_num = (first + second); //keep track of the lowest output
                    lowest_idx = i; //keep track of the index of the lowest output
                }
            }
        }

        player_lives[lowest_idx] -= 1;
        printf("%s is forced to eat garlic!\n", names[lowest_idx]);

        switch (player_lives[lowest_idx]) {
        case (0): printf("%s has died.\n", names[lowest_idx]); break;
        case (1):
            printf("%s has %d life remaining.\n", names[lowest_idx], player_lives[lowest_idx]);
            break;
        default:
            printf("%s has %d lives remaining.\n", names[lowest_idx], player_lives[lowest_idx]);
            break;
        }

        for (int i = 0; i < total_player; i++) {
            if (player_lives[i] == 0) {
                death_counter += 1;
            }
        }

        if (death_counter == (total_player - 1)) {
            break;
        }

        round_counter += 1;
    }

    for (int i = 0; i < total_player; i++) {
        if (player_lives[i] != 0) {
            printf("%s wins the Garlic Game!\n", names[i]);
        }
    }
}

// this function will calculate the index number of
// the player on the left
int left(int pos, int total_player) {
    return (pos + total_player - 1) % total_player;
}

// this function will calculate the index number of
// the player on the right
int right(int pos, int total_player) {
    return (pos + 1) % total_player;
}
