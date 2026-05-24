#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "types.h"

Player player[NUM_OF_PLAYERS];

int STARTING_POSITION[] = {0, 13, 26, 39};
int APPROACH_CELL[] = {50, 11, 24, 37};

void printStart() {
    printf("The Yellow player has four (04) pieces named Y1, Y2, Y3, and Y4.\n");
    printf("The Blue player has four (04) pieces named B1, B2, B3, and B4.\n");
    printf("The Red player has four (04) pieces named R1, R2, R3, and R4.\n");
    printf("The Green player has four (04) pieces named G1, G2, G3, and G4.\n\n");
}

const char* printPlayer(int color) {
    const char* players[] = {"Yellow", "Blue", "Red", "Green"};
    return players[color];
}

void setOrder(int startingPlayer, int order[]) {  //Setting the order of play
    printf("The order of a single round is ");
    for (int i = 0; i < NUM_OF_PLAYERS; i++) {
        order[i] = ((startingPlayer + i) % NUM_OF_PLAYERS);
        printf("%s", printPlayer(order[i]));
        if (i < NUM_OF_PLAYERS - 1) {
            if (i == 2) {
                printf(" & ");
            } else {
                printf(", ");
            }
        }
    }
    printf(".\n\n");
}

int rolldice() {   //Get the dice value (1-6)
    return ((rand() % 6) + 1);
}

int choiceRoll(int num) {  //Get a random number between a given range according to situation
    return (rand() % num);
}

int firstChance() {  //Choose who will start the game
    int rolls[NUM_OF_PLAYERS];
    int maxValue = 0;
    int maxPlayer;
    for (int i = 0; i < NUM_OF_PLAYERS; i++) {
        rolls[i] = ((rand() % 6) + 1);
        printf("%s rolls %d.\n", printPlayer(i), rolls[i]);

        if (rolls[i] > maxValue) {
            maxValue = rolls[i];
            maxPlayer = i;
        }
    }
    return maxPlayer;
}

bool allAtHome(Player* player) {  //To check whether the player has finished the game
    for (int j = 0; j < PIECES_PER_PLAYER; j++) {
        if (!player->pieces[j].atHome) {
            return false;
        }
    }
    return true;
}

void initialize(Player player[NUM_OF_PLAYERS]) {
    for (int i = 0; i < NUM_OF_PLAYERS; i++) { //Initialize player characteristics
        player[i].color = i;
        player[i].numOfPiecesAtHome = 0;
        player[i].numOfPiecesInBase = 4;
        player[i].numOfPiecesOnBoard = 0;
        player[i].finishedTheGame = false;
        for (int j = 0; j < PIECES_PER_PLAYER; j++) {//Initialize relevant player's piece characteristics
            player[i].pieces[j].position = -1;
            player[i].pieces[j].inBase = true;
            player[i].pieces[j].atHome = false;
            player[i].pieces[j].playing = false;
        }
    }
}

void movePiece(Player* player, int pieceIndex, int roll) {
    if (player->pieces[pieceIndex].playing) {
        int newPosition;    
        int homePosition = APPROACH_CELL[player->color] + HOME_PATH;
    
        //Below if block handles the movements in the home path including reaching home
        if (player->pieces[pieceIndex].inHomePath) {
            if (player->pieces[pieceIndex].position + roll == homePosition) {
                player->pieces[pieceIndex].position = homePosition;
                player->pieces[pieceIndex].atHome = true;
                player->pieces[pieceIndex].playing = false;
                (player->numOfPiecesAtHome)++;
                (player->numOfPiecesOnBoard)--;
                printf("%s player's piece %d has reached home!\n", printPlayer(player->color), (pieceIndex + 1));
            } else if (player->pieces[pieceIndex].position + roll > homePosition) { //Making sure that piece should get exact roll to reach home
                printf("%s player needs an exact roll to reach home!\n", printPlayer(player->color));
            } else {
                //movements in home path
                player->pieces[pieceIndex].position += roll;
                printf("%s player moves piece %d to cell %d in home path.\n", printPlayer(player->color), (pieceIndex + 1), player->pieces[pieceIndex].position - APPROACH_CELL[player->color]);
            }
        } else {
            //Below code block checks for the pieces entering approaching cells and home path
            if (player->pieces[pieceIndex].position < APPROACH_CELL[player->color] && 
                player->pieces[pieceIndex].position + roll >= APPROACH_CELL[player->color]) {
                player->pieces[pieceIndex].position = APPROACH_CELL[player->color];
                player->pieces[pieceIndex].inHomePath = true;
                printf("%s player's piece %d has entered the approach cell %d.\n", printPlayer(player->color), (pieceIndex + 1), APPROACH_CELL[player->color]);
            } else {
                newPosition = (player->pieces[pieceIndex].position + roll) % BOARD_SIZE;

                // cheacks for capturing another player's piece
                bool capture = false;
                for (int i = 0; i < NUM_OF_PLAYERS; i++) {
                    if (i != player->color) { 
                        for (int j = 0; j < PIECES_PER_PLAYER; j++) {
                            if (player[i].pieces[j].playing && player[i].pieces[j].position == newPosition) {
                                player[i].pieces[j].position = -1;
                                player[i].pieces[j].inBase = true;
                                player[i].pieces[j].playing = false;
                                (player[i].numOfPiecesInBase)++;
                                (player[i].numOfPiecesOnBoard)--;
                                printf("%s player captures %s player's piece %d. Returns it to base.\n", printPlayer(player->color), printPlayer(i), (j + 1));
                                capture = true;
                                break;
                            }
                        }
                        if (capture) break;
                    }
                }

                //checks for the blocks on the path
                if (!capture) {
                    for (int j = 0; j < PIECES_PER_PLAYER; j++) {
                        if (j != pieceIndex && player->pieces[j].playing && player->pieces[j].position == newPosition) {
                            printf("%s player's piece %d is blocked from moving to cell %d by another piece of the same color.\n", printPlayer(player->color), pieceIndex + 1, newPosition);
                            return;
                        }
                    }
                    player->pieces[pieceIndex].position = newPosition;
                    printf("%s player moves piece %d to cell %d.\n", printPlayer(player->color), (pieceIndex + 1), newPosition);
                }
            }
        }
    } else if (roll == 6) {
        //Below code block acts on getting a piece out from base to starting point
        player->pieces[pieceIndex].position = STARTING_POSITION[player->color];
        player->pieces[pieceIndex].inBase = false;
        player->pieces[pieceIndex].playing = true;
        (player->numOfPiecesInBase)--;
        (player->numOfPiecesOnBoard)++;
        printf("%s player moves piece %d to the starting point.\n", printPlayer(player->color), (pieceIndex + 1));
    } else {
        printf("%s player cannot move piece %d (needs a 6).\n", printPlayer(player->color), (pieceIndex + 1));
    }
}

void playerStatus(Player player[NUM_OF_PLAYERS]) {
    printf("----------------------------------------------------------------------\n");
    for (int i = 0; i < NUM_OF_PLAYERS; i++) {
        printf("%s player now has %d/4 pieces on the board and %d/4 pieces in the base.\n", printPlayer(player[i].color), player[i].numOfPiecesOnBoard, player[i].numOfPiecesInBase);
        printf("=========================================================================\n");
        printf("Location of %s's pieces\n", printPlayer(player[i].color));
        printf("=========================================================================\n");
        for (int j = 0; j < PIECES_PER_PLAYER; j++) {
            if (player[i].pieces[j].inBase) {
                printf("piece %d --> in Base\n", (j + 1));
            } else if (player[i].pieces[j].atHome) {
                printf("Piece %d --> at Home\n", (j + 1));
            } else {
                printf("Piece %d --> at cell %d\n", (j + 1), player[i].pieces[j].position);
            }
        }
        printf("----------------------------------------------------------------------\n");
    }
}

void gamePlay(Player* player) {
    int b2bSixes = 0;  //declare a variable that stores back to back sixes count
    bool extraTurn = true;

    while (extraTurn) {
        int roll = rolldice();
        printf("%s player rolls %d.\n", printPlayer(player->color), roll);
        player->diceValue = roll;
        b2bSixes++;

        if (b2bSixes == 3 && roll == 6) {  //If player rolls 3 consecutive times and rolls 6 in the third roll ,roll is ignored.
            extraTurn = false;
            break;
        }

        if (roll == 6) {  
            if (player->numOfPiecesInBase > 0 && player->numOfPiecesOnBoard > 0) {
                //When player rolls 6 and there are pieces in both base and board,
                //game randomly chooses whether a piece gets out of base or move a piece already on board.
                int choice = choiceRoll(2);
                if (choice == 1) {
                    for (int i = 0; i < PIECES_PER_PLAYER; i++) { //If random Choice is 1 gets a piece out of the base to the starting point
                        if (player->pieces[i].inBase) {
                            movePiece(player, i, roll);
                            break;
                        }
                    }
                } else {
                    int playingPieces[PIECES_PER_PLAYER];
                    int count = 0;
                    for (int i = 0; i < PIECES_PER_PLAYER; i++) {
                        if (player->pieces[i].playing && !player->pieces[i].inBase) {
                            playingPieces[count++] = i;
                        }
                    }
                    if (count > 0) {
                        int randomIndex = choiceRoll(count); //Randomly choose a piece which is already on the board
                        movePiece(player, playingPieces[randomIndex], roll);
                    }
                }
            } else if (player->numOfPiecesInBase > 0) {
                for (int i = 0; i < PIECES_PER_PLAYER; i++) {
                    if (player->pieces[i].inBase) {
                        movePiece(player, i, roll);
                        break;
                    }
                }
            } else {
                int onBoardPieces[PIECES_PER_PLAYER];
                int count = 0;
                for (int i = 0; i < PIECES_PER_PLAYER; i++) {
                    if (player->pieces[i].playing) {
                        onBoardPieces[count++] = i;
                    }
                }
                if (count > 0) {
                    int randomIndex = choiceRoll(count);
                    movePiece(player, onBoardPieces[randomIndex], roll);
                }
            }
            extraTurn = true;
        } else {
            int onBoardPieces[PIECES_PER_PLAYER];
            int count = 0;
            for (int i = 0; i < PIECES_PER_PLAYER; i++) {
                if (player->pieces[i].playing) {
                    onBoardPieces[count++] = i;
                }
            }
            if (count > 0) {
                int randomPiece = choiceRoll(count);
                movePiece(player, onBoardPieces[randomPiece], roll);
            }
            extraTurn = false; 
        }

        if (allAtHome(player)) {
            player->finishedTheGame = true;
            extraTurn = false;
        }
    }
}
