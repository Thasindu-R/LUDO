#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "types.h"

int main() {
    srand(time(0));
    int order[NUM_OF_PLAYERS];
    int startingPlayer = firstChance();
    setOrder(startingPlayer, order);
    initialize(player);
    printStart();

    bool gameOver = false;
    int round = 1;
    int winners[NUM_OF_PLAYERS];
    int winnerCount = 0;

    while (!gameOver) {
        printf("\nRound %d:\n", round);
        for (int i = 0; i < NUM_OF_PLAYERS; i++) {
            if (!player[order[i]].finishedTheGame) {
                gamePlay(&player[order[i]]);

                if (player[order[i]].finishedTheGame && winnerCount < NUM_OF_PLAYERS) {
                    winners[winnerCount] = order[i];
                    winnerCount++;
                    printf("\n%s player has finished the game!\n", printPlayer(order[i]));
                }
            }
        }
        playerStatus(player);

        if (winnerCount == NUM_OF_PLAYERS) {
            gameOver = true;
        }
        round++;
    }

    printf("\n======================================================================\n");
    printf("|                              Game Over                             |\n");
    printf("======================================================================\n\n");
    printf("Total rounds: %d\n",round-=1);
    for (int i = 0; i < NUM_OF_PLAYERS; i++) {
        printf("%s player   ->  %d place\n", printPlayer(winners[i]),i+1);
    }

    return 0;
}