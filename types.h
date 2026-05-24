#ifndef TYPES_H
#define TYPES_H

#include <stdbool.h>

#define NUM_OF_PLAYERS 4
#define PIECES_PER_PLAYER 4
#define BOARD_SIZE 52
#define HOME_PATH 6

typedef struct{
    int position;
    bool inBase;
    bool inHomePath;
    bool playing;
    bool atHome;
}Piece;

typedef struct{
    int color;
    Piece pieces[PIECES_PER_PLAYER];
    int numOfPiecesOnBoard;
    int numOfPiecesInBase;
    int numOfPiecesAtHome;
    int diceValue;
    bool finishedTheGame;
}Player;

extern Player player[NUM_OF_PLAYERS];

extern int STARTING_POSITIONS[];
extern int APPROACH_CELLS[];

void printStart();
const char* printPlayer(int color);
void setOrder(int startingPlayer, int order[]);
int rolldice();
int choiceRoll(int num);
int firstChance();
bool allAtHome(Player* player);
void initialize(Player player[NUM_OF_PLAYERS]);
void movePiece(Player* player, int pieceIndex, int roll);
void playerStatus(Player player[NUM_OF_PLAYERS]);
void gamePlay(Player* player);

#endif