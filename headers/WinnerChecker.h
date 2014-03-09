#ifndef _WINNER_CHECKER_H_
#define _WINNER_CHECKER_H_

class BaseGame;

#include "BaseGame.h"

class WinnerChecker
{
    BaseGame *baseGame;

    int totalCells;
    int filledCells;

    bool checkColumnAlign(int);
    bool checkRowAlign(int);

    int winnerId;
public:
    WinnerChecker(BaseGame *);
    void onPlaceToken(int, int);
    void onRemoveToken(int, int);
    void onRotate();

    int getWinnerId();
};

#endif