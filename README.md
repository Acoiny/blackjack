# Blackjack-cli
A simple version of blackjack, playable in the cli.
Cards are randomized and shuffled via the fisher-yates algorithm.
The programs return code signals if the player has won the round, so
the game's return code can be used in bash scripts.
```
$ blackjack --help
usage: blackjack [options]
    options can be of the following

    -h              prints this help message
    --help          same as -h
    -s              play a small game, with only one deck
    --small         same as -s
    -d <n>          use n decks for the game
    --decks <n>     same as -d <n>
    --print-cards   print all the cards in the deck
                    in a readable format
```
NOTE: This project is not complete and probably has some flaws.
Also, it doesn't comply 100% to the real blackjack rules.
