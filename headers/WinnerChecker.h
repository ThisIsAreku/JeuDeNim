#ifndef _WINNER_CHECKER_H_
#define _WINNER_CHECKER_H_

class Game;

#include "Game.h"

#include "structs/TokenAlignement.h"

class WinnerChecker
{
    Game *game;
    const Grid *grid;

    bool useGraphics;

    bool *winner;
    int *winAlignementsCount;
    TokenAlignement **winAlignements;

    int *alignementsCount;
    int *maxAlignementsSizes;

    bool winnerFlag;
    bool drawFlag;
    int minCells;

    void resetWinner();
    void resetWinner(int);

    void checkDiagonalsAlign();
    void checkColumnsAlign();
    void checkRowsAlign();

    void checkDiagonal1Align(int, int);
    void checkDiagonal2Align(int, int);
    void checkColumnAlign(int);
    void checkRowAlign(int);
    
    void searchAlign(int, int, int &, int &, int &, int &);

    void updateFlags();


public:
    WinnerChecker(Game *, bool);
    ~WinnerChecker();

    void setGridToCheck(const Grid *);

    void searchWinner(Grid *grid = NULL, bool forceCheck = false);
    bool fastWinCheck(Grid *grid = NULL, bool forceCheck = false);
    void resetWinAlignementsCount();

    int getWinnerId(int) const;
    int getNumWinner() const;
    int getMaxAlignSize(int) const;
    int getNumAlign(int) const;
    TokenAlignement *getWinAlignement(int) const;
    int getNumWinAlignements(int) const;

    bool isWinner(int) const;

    bool hasWinner() const;
    bool hasDraw() const;

};

#endif
