#include "WinnerChecker.h"

WinnerChecker::WinnerChecker(BaseGame *baseGame)
{
    this->baseGame = baseGame;
    this->winnerId = 0;
    this->totalCells = baseGame->getBaseGrid()->getWidth() * baseGame->getBaseGrid()->getHeight();
    this->filledCells = 0;
}

bool WinnerChecker::checkColumnAlign(int x)
{
    if(getWinnerId() != -1)
        return false;
    if(this->filledCells < baseGame->getGameSettings()->getAlignSize())
        return false;

    int win = -1;
    int alc = 0;
    int ccell;
    for (int y = 0; y < baseGame->getBaseGrid()->getHeight(); ++y)
    {
        ccell = baseGame->getBaseGrid()->getGridAt(x, y);
        if(ccell <= 0)
            continue;
        if(win != ccell)
        {
            win = ccell;
            alc = 1;
        }
        else
        {
            alc++;
            if(alc == baseGame->getGameSettings()->getAlignSize())
            {
                this->winnerId = win;
                return false;
            }
        }
    }
    std::cerr << "checkColumnAlign: " << alc << std::endl;
    return true;
}
bool WinnerChecker::checkRowAlign(int y)
{
    if(getWinnerId() != -1)
        return false;
    if(this->filledCells < baseGame->getGameSettings()->getAlignSize())
        return false;

    int win = -1;
    int alc = 0;
    int ccell;
    for (int x = 0; x < baseGame->getBaseGrid()->getWidth(); ++x)
    {
        ccell = baseGame->getBaseGrid()->getGridAt(x, y);
        if(ccell <= 0)
            continue;
        if(win != ccell)
        {
            win = ccell;
            alc = 1;
        }
        else
        {
            alc++;
            if(alc == baseGame->getGameSettings()->getAlignSize())
            {
                this->winnerId = win;
                return false;
            }
        }
    }
    std::cerr << "checkRowAlign: " << alc << std::endl;
    return true;
}



void WinnerChecker::onPlaceToken(int x, int y)
{
    this->filledCells++;
    checkColumnAlign(x) && checkRowAlign(y);
}
void WinnerChecker::onRemoveToken(int x, int y)
{
    this->filledCells--;
    checkColumnAlign(x) && checkRowAlign(y);
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