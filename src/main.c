#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "game.h"
#include "options.h"

#define printError(str, ...) fprintf(stderr, str, __VA_ARGS__)

const char *short_options = "hsd:v";

struct _Options Options = {
    .nDecks = 6,
    .print_Cards = 0,
    .verbose = 0,
};

const struct option long_options[] = {
    {"help", 0, NULL, 'h'},
    {"small", 0, NULL, 's'},
    {"decks", 1, NULL, 'd'},
    {"verbose", 0, NULL, 'v'},
    {"print-cards", 0, &Options.print_Cards, 1},
    {NULL, 0, NULL, 0}};

static int processOptions(int argc, char **argv)
{
    bool continueAfterOptions = true;
    char opt;

    while (-1 !=
           (opt = getopt_long(argc, argv, short_options, long_options, NULL)))
    {
        switch (opt)
        {
        case 0:
        {
            // longoption has set the flag
            continueAfterOptions = false;
            break;
        }
        case 'h':
        {
            printf("%s", helpMessage);
            continueAfterOptions = false;
            goto endOptions;
        }
        case 'd':
        {
            char *endptr;
            int decks = strtol(optarg, &endptr, 10);
            if (endptr == optarg || *endptr != '\0')
            {
                printError("'%s' is not a number\n", optarg);
                continueAfterOptions = false;
                goto endOptions;
            }
            else
            {
                Options.nDecks = decks;
            }
            break;
        }
        case 's':
        {
            Options.nDecks = 1;
            break;
        }
        case 'v':
        {
            Options.verbose = 1;
            break;
        }
        case '?':
        {
            fprintf(stderr, "Unknown option\n");
            continueAfterOptions = false;
            goto endOptions;
        }
        default:
        {
            fprintf(stderr, "Shouldn't get here!!!\n");
            continueAfterOptions = false;
            goto endOptions;
        }
        }
    }

endOptions:
    return continueAfterOptions;
}

int main(int argc, char **argv)
{
    bool rungame = processOptions(argc, argv);

    // initialize timer
    srand(time(NULL));

    uint32_t nCards;
    Card *cards = getStack(Options.nDecks, &nCards);

    uint8_t player_won = 0;
    if (rungame)
        player_won = startRound(cards, nCards);
    else
    {
        if (Options.print_Cards)
        {
            printCardStack(cards, nCards);
        }

        player_won = 2;
    }
    free(cards);

    return player_won;
}
