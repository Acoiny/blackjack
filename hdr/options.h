#pragma once

extern const char *helpMessage;

struct _Options
{
    int nDecks;
    int print_Cards;
    int verbose;
};

extern struct _Options Options;
