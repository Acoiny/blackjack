#include "game.h"

#include "options.h"

#include <stdio.h>
#include <stdlib.h>

const char *CardColors[] = {"Heart", "Spade", "Clubs", "Diamond"};

const char *CardValueNames[] = {[TWO] = "two", "three", "four", "five", "six",
                                "seven",       "eight", "nine", "ten",  "jack",
                                "queen",       "king",  "ace"};

static const char *stringifyCard(Card *card)
{
    static char buf[64];
    snprintf(buf, 64, "%s of %s", CardValueNames[card->value], card->color);

    return buf;
}

/*
 * Hacky way to print a stack of cards
 */
void printCardStack(Card *cards, int n)
{
    for (int i = 0; i < n; ++i)
    {
        printf("%s\n", stringifyCard(&cards[i]));
    }
}

/*
 * Filles the cards array up with 52 cards
 * aka a whole deck of cards
 *
 * To have 6 decks, simply call this function
 * multiple times, with the pointer offset
 * by a whole deck
 */
static void fillDeck(Card *cards)
{
    CardValue val = TWO;
    uint8_t colorIndex = 0;

    for (int i = 0; i < 52; ++i)
    {
        const char *color = CardColors[colorIndex];
        // colors rotating between the 4
        cards[i].color = color;
        cards[i].value = val;

        if (val == ACE)
        {
            val = TWO;
            colorIndex++;
        }
        else
        {
            val++;
        }
    }
}

/*
 * https://en.wikipedia.org/wiki/Fisher%E2%80%93Yates_shuffle
 * Shuffles cards via the Fisher-Yates shuffle
 */
static void shuffleCards(Card cards[], uint16_t nCards)
{
    for (int i = nCards - 1; i > 0; i--)
    {
        uint16_t j = rand() % (i + 1);
        swap(cards[i], cards[j]);
    }
}

/*
 * Calculates the points an actor
 * currently has and stores
 * it inside the Actor struct
 *
 * @param actor
 */
static void calcPoints(Actor *actor)
{
    // keep track of number of aces
    uint8_t nAces = 0;

    actor->points = 0;

    for (int i = 0; i < actor->n_cards; ++i)
    {
        Card *c = &actor->cards[i];
        uint8_t value = c->value;

        if (value == ACE)
        {
            nAces++;
            actor->points += 11;
        }
        else
        {
            actor->points += (value >= 10 ? 10 : value);
        }
    }

    // change aces values from 11 to 1, if player has too many points
    for (int i = 0; i < nAces; ++i)
    {
        // subtract 10 points, setting this ace's value to 1
        if (actor->points > 21)
            actor->points -= 10;
    }
}

/**
 * @brief draws a random card and gives it to the provided actor
 *
 * @param actor the actor, to give the card to
 * @param cards the stack of cards
 *
 * @returns a pointer to the newly drawn card inside
 * the Actor struct
 */
static void drawCard(Actor *actor, Card *cards, uint16_t nCards)
{
    // getting a random index for the stack of cards
    uint16_t index = rand() % (nCards + 1);
    uint16_t firstIndex = index;

    // if there is no card at this place, linear probe
    while (cards[index].value == EMPTY)
    {
        index++;
        // to prevent infinite loops, check if all the cards have been
        // searched through
        //
        // as this should never happen, exit program and print error
        if (index == firstIndex)
        {
            fprintf(stderr, "drawCard: all cards have been searched!\n");
            exit(1);
        }
    }

    // give card to actor and set stack slot to empty
    actor->cards[actor->n_cards++] = cards[index];
    cards[index].value = EMPTY;

    calcPoints(actor);
}

/*
 * @brief Plays the dealers play.
 * the dealer should have two cards already when calling this function
 * and will continue to draw cards, until he has
 */
static void dealerPlay(Actor *dealer, Card *cards, uint16_t nCards)
{
    if (Options.verbose)
        printf("The dealer's second card is the %s, he has %d points\n",
               stringifyCard(&dealer->cards[dealer->n_cards - 1]),
               dealer->points);

    while (dealer->points < 17)
    {
        drawCard(dealer, cards, nCards);

        if (Options.verbose)
            printf("The dealer drew the %s and has %d points\n",
                   stringifyCard(&dealer->cards[dealer->n_cards - 1]),
                   dealer->points);
    }

    if (!Options.verbose)
        printf("dealer: %d\n", dealer->points);
    else
        printf("The dealer stopped drawing with %d points\n", dealer->points);
}

Card *getStack(uint8_t nDecks, uint32_t *nCards)
{
    *nCards = nDecks * 52;

    Card *cards = malloc(sizeof(Card) * (*nCards));

    // fill the card-decks, depending on the round size
    for (int i = 0; i < nDecks; ++i)
    {
        fillDeck(cards + (52 * i));
    }

    shuffleCards(cards, *nCards);

    return cards;
}

int startRound(Card *cards, uint32_t nCards)
{
    Actor player = {.points = 0, .n_cards = 0},
          dealer = {.points = 0, .n_cards = 0};

    // start of the round
    drawCard(&player, cards, nCards);
    Card *current = &player.cards[player.n_cards - 1];

    if (!Options.verbose)
        printf("%s %d\n", stringifyCard(current),
               player.points);
    else
        printf("You drew the %s\n%d points\n", stringifyCard(current),
               player.points);

    drawCard(&dealer, cards, nCards);
    current = &dealer.cards[dealer.n_cards - 1];
    if (!Options.verbose)
        printf("dealer: %d\n", dealer.points);
    else
        printf("The dealer drew the %s\n%d points\n", stringifyCard(current),
               dealer.points);

    drawCard(&player, cards, nCards);
    current = &player.cards[player.n_cards - 1];
    if (!Options.verbose)
        printf("%s %d\n", stringifyCard(current),
               player.points);
    else
        printf("You drew the %s\n%d points\n", stringifyCard(current),
               player.points);

    drawCard(&dealer, cards, nCards);
    if (!Options.verbose)
        printf("dealer: %d\n", dealer.points);
    else
        printf("The dealer drew his second card, and placed it face down\n");

    char *line = NULL;
    size_t buffLen = 0;

    for (;;)
    {
        if (!Options.verbose)
            printf("draw another?(y/N)");
        else
            printf("Do you want to draw another card?(y/N)");

        size_t len = getline(&line, &buffLen, stdin);

        if (len > 0 && (line[0] == 'y' || line[0] == 'Y'))
        {
            drawCard(&player, cards, nCards);
            current = &player.cards[player.n_cards - 1];

            if (!Options.verbose)
                printf("%s %d\n", stringifyCard(current), player.points);
            else
                printf("You drew the %s\n%d points\n", stringifyCard(current),
                       player.points);

            if (player.points > 21)
                break;
        }
        else
        {
            if (Options.verbose)
                printf("You stopped with %d points\n", player.points);
            break;
        }
    }

    if (line)
        free(line);

    uint8_t returnCode = WIN;

    if (player.points > 21)
    {
        if (!Options.verbose)
            printf("bust\n");
        else
            printf("You went 'bust' and lost\n");
        returnCode = BUST;
    }
    else
    {
        dealerPlay(&dealer, cards, nCards);

        if (dealer.points > 21)
        {
            if (!Options.verbose)
                printf("win\n");
            else
                printf("The dealer wet 'bust', you won!\n");
            returnCode = WIN;
        }
        else if (player.points > dealer.points)
        {
            if (!Options.verbose)
                printf("win\n");
            else
                printf("You(%d) got more points than the dealer(%d) and won\n",
                       player.points, dealer.points);
            returnCode = WIN;
        }
        else
        {
            if (!Options.verbose)
                printf("lose\n");
            else
                printf("You(%d) got less points than the dealer(%d) and lost\n",
                       player.points, dealer.points);
            returnCode = LOWER_THAN_DEALER;
        }
    }

    return returnCode;
}
