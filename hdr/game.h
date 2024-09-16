#pragma once

#include <stdint.h>

// generic swap macro
#define swap(a, b)                                                             \
    do                                                                         \
    {                                                                          \
        typeof(a) _tmp = a;                                                    \
        a = b;                                                                 \
        b = _tmp;                                                              \
    } while (0)

enum ReturnCodes
{
    WIN,
    BUST,
    LOWER_THAN_DEALER
};

typedef enum _CardValue
{
    EMPTY, // used for already dealt cards

    TWO = 2,
    THREE,
    FOUR,
    FIVE,
    SIX,
    SEVEN,
    EIGHT,
    NINE,
    TEN,
    JACK,
    QUEEN,
    KING,
    ACE,

    CARDVALUE_END // dummy, to use in loops
} CardValue;

typedef struct _Card
{
    CardValue value;
    const char *color;
} Card;

// maximum of cards is theoretically 11
// as 11 * 2 (smallest value) = 22
#define MAX_CARDS 11

typedef struct _Actor
{
    uint8_t points;
    Card cards[MAX_CARDS];
    uint8_t n_cards;
} Actor;

extern const char *CardColors[];

// the string equivalent to the CardValue enum
// can be indexed with the enum names
extern const char *CardValueNames[];

/**
 * @brief Prints a stack of cards in a readable format
 *
 * @param cards pointer to the stack of cards
 * @param n size of the stack
 *
 */
void printCardStack(Card *cards, int n);

/*
 * @brief allocates a stack of cards
 * and returns it
 *
 * @param nDecks number of decks to use
 *
 * @returns an allocated stack of cards
 */
Card* getStack(uint8_t nDecks, uint32_t *nCards);

/*
 * Runs a round of blackjack
 * @param nDecks number of decks to use
 *
 * @returns 1 if the round has been won
 */
int startRound(Card* cards, uint32_t nCards);
