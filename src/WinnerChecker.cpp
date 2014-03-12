#include "WinnerChecker.h"

WinnerChecker::WinnerChecker(BaseGame *baseGame)
{
    this->baseGame = baseGame;
    this->winnerId = 0;
    this->totalCells = baseGame->getBaseGrid()->getWidth() * baseGame->getBaseGrid()->getHeight();
    this->filledCells = 0;
}

void WinnerChecker::checkColumnAlign(int x)
{
    if(getWinnerId() != -1)
        return;
    if(this->filledCells < baseGame->getGameSettings()->getAlignSize())
        return;

    int win = -1;
    int alc = 0;
    for (int y = 0; y < baseGame->getBaseGrid()->getHeight(); ++y)
    {
        if(searchAlign(x, y, win, alc))
            return;
    }
    //std::cerr << "checkColumnAlign: " << alc << std::endl;
}
void WinnerChecker::checkRowAlign(int y)
{
    if(getWinnerId() != -1)
        return;
    if(this->filledCells < baseGame->getGameSettings()->getAlignSize())
        return;

    int win = -1;
    int alc = 0;
    for (int x = 0; x < baseGame->getBaseGrid()->getWidth(); ++x)
    {
        if(searchAlign(x, y, win, alc))
            return;
    }
    //std::cerr << "checkRowAlign: " << alc << std::endl;
}

bool WinnerChecker::searchAlign(int x, int y, int &win, int &alc)
{
    int ccell = baseGame->getBaseGrid()->getGridAt(x, y);
    if(win != ccell || ccell <= 0)
    {
        win = ccell;
        alc = 1;
    }
    else
    {
        alc++;
        if(alc == baseGame->getGameSettings()->getAlignSize())
        {
            std::cerr << "Winner is " << win << " with last @ " << x << "," << y << " W " << alc << std::endl;
            this->winnerId = win;
            return true;
        }
    }
    return false;
}

void WinnerChecker::onPlaceToken(int x, int y)
{
    std::cerr << "WinnerChecker::onPlaceToken: " << x << "," << y << std::endl;
    this->filledCells++;
    checkColumnAlign(x);
    checkRowAlign(y);
}
void WinnerChecker::onRemoveToken(int x, int y)
{
    std::cerr << "WinnerChecker::onRemoveToken: " << x << "," << y << std::endl;
    this->filledCells--;
    for (int i = 0; i <= y; ++i)
    {
        checkRowAlign(i);
    }
    checkColumnAlign(x);
}
void WinnerChecker::onRotate()
{
    std::cerr << "WinnerChecker::onRotate" <<  std::endl;
    for (int i = 0; i < baseGame->getBaseGrid()->getWidth(); ++i)
    {
        checkColumnAlign(i);
    }
    for (int i = 0; i < baseGame->getBaseGrid()->getHeight(); ++i)
    {
        checkRowAlign(i);
    }
}

int WinnerChecker::getWinnerId()
{
    return winnerId - 1;
}