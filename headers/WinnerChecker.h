#ifndef _WINNER_CHECKER_H_
#define _WINNER_CHECKER_H_

class Game;

#include "Game.h"

#include "structs/TokenAlignement.h"

class WinnerChecker
{
    Game *game;

    bool useGraphics;

    bool *winner;
    int *winAlignementsCount;
    TokenAlignement **winAlignements;

    bool winnerFlag;
    bool drawFlag;

    int totalCells;
    int filledCells;
    int minCells;

    void resetWinner();
    void resetWinner(int);
    void resetWinAlignementsCount();

    void checkDiagonalAlign();
    void checkDiagonal1Align(int, int);
    void checkDiagonal2Align(int, int);
    void checkColumnAlign(int);
    void checkRowAlign(int);
    void searchAlign(int, int, int &, int &, int &, int &);

    void updateFlags();
public:
    WinnerChecker(Game *, bool);
    ~WinnerChecker();
    void onPlaceToken(int, int);
    void onRemoveToken(int, int);
    void onRotate();

    int getWinnerId(int);
    int getNumWinner();
    TokenAlignement *getWinAlignement(int);

    bool hasWinner();
    bool hasDraw();
};

#endif