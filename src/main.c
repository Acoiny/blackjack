#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef enum _CardValue {
    TWO = 2,
    THREE,
    FOUR,
    FIVE,
    SIX,
    SEVEN,
    EIGHT,
    NINE,
    TEN,
    
} CardValue;

typedef struct
{
    uint8_t points;
} Actor;


/*
 * Runs a round of blackjack
 *
 * @returns 1 if the round has been won
 */
static int startRound()
{
    Actor player = {.points = 0}, dealer = {.points = 0};

    return 0;
}

int main(int argc, char **argv)
{
    // initialize timer
    srand(time(NULL));

    uint8_t player_won = startRound();

    return 0;
}
