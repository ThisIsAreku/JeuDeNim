#include "WinnerChecker.h"



#include "Logger.h"

WinnerChecker::WinnerChecker(BaseGame *baseGame, bool useGraphics)
{
    this->useGraphics = useGraphics;
    this->baseGame = baseGame;
    this->totalCells = baseGame->getBaseGrid()->getWidth() * baseGame->getBaseGrid()->getHeight();
    this->filledCells = 0;
    //nombre minimal de jetons pour un alignement
    this->minCells = (baseGame->getGameSettings()->getAlignSize() * baseGame->getGameSettings()->getNumAlign()) - (baseGame->getGameSettings()->getNumAlign() - 1);
    this->winner = new bool[baseGame->getGameSettings()->getNumPlayers()];
    resetWinner();

    winAlignementsCount = new int[baseGame->getGameSettings()->getNumPlayers()];

    if(useGraphics)
    {
        winAlignements = new TokenAlignement*[baseGame->getGameSettings()->getNumPlayers()];
        for (int i = 0; i < baseGame->getGameSettings()->getNumPlayers(); ++i)
            winAlignements[i] = new TokenAlignement[baseGame->getGameSettings()->getAlignSize()];
    }
    t = NULL;
}
WinnerChecker::~WinnerChecker()
{
    delete [] this->winner;
    if(useGraphics)
    {
        for (int i = 0; i < baseGame->getGameSettings()->getNumPlayers(); ++i)
        {
            delete [] winAlignements[i];
        }
        delete [] winAlignements;
    }
}

void WinnerChecker::resetWinAlignementsCount()
{
    for(int i = 0; i < baseGame->getGameSettings()->getNumPlayers(); i++)
        winAlignementsCount[i] = 0;
}

void WinnerChecker::resetWinner()
{
    for(int i = 0; i < baseGame->getGameSettings()->getNumPlayers(); i++)
        resetWinner(i);

    drawFlag = false;
    winnerFlag = false;
}
void WinnerChecker::resetWinner(int i)
{
    this->winner[i] = false;
}

void WinnerChecker::updateFlags()
{
    int nbrWinner = 0;
    winnerFlag = false;
    for (int i = 0; i < baseGame->getGameSettings()->getNumPlayers(); ++i)
    {
        if(winAlignementsCount[i] == baseGame->getGameSettings()->getNumAlign())
        {
            this->winner[i] = true;
            nbrWinner++;
            winnerFlag = true;
        }
        else
        {
            this->winner[i] = false;
        }
    }
    if(nbrWinner == baseGame->getGameSettings()->getNumPlayers())
        drawFlag = true;
    else
        drawFlag = false;
}

void WinnerChecker::checkColumnAlign(int x)
{
    if(getWinnerId() != -1)
        return;
    if(this->filledCells < this->minCells)
        return;

    int win = -1;
    int alc = 0;
    int fx, fy;
    for (int y = 0; y < baseGame->getBaseGrid()->getHeight(); ++y)
    {
        searchAlign(x, y, win, alc, fx, fy);
    }
    //Logger::log << "checkColumnAlign: " << alc << std::endl;
}
void WinnerChecker::checkDiagonalAlign()
{
    if(getWinnerId() != -1)
        return;
    if(this->filledCells < this->minCells)
        return;

    for (int v = 0; v < baseGame->getBaseGrid()->getWidth(); ++v)
    {
        checkDiagonal1Align(v, 0);
        checkDiagonal2Align(v, 0);
    }
    for (int v = 0; v < baseGame->getBaseGrid()->getHeight(); ++v)
    {
        checkDiagonal1Align(0, v);
        checkDiagonal2Align(0, v);
    }
    //Logger::log << "checkColumnAlign: " << alc << std::endl;
}
void WinnerChecker::checkDiagonal1Align(int x, int y) // l->r
{
    int win = -1;
    int alc = 0;
    int fx, fy;
    while((x < baseGame->getBaseGrid()->getWidth() && y < baseGame->getBaseGrid()->getWidth()) && (x >= 0 && y >= 0))
    {
        searchAlign(x++, y++, win, alc, fx, fy);
    }
    //Logger::log << "checkColumnAlign: " << alc << std::endl;
}
void WinnerChecker::checkDiagonal2Align(int x, int y) // r->l
{
    int win = -1;
    int alc = 0;
    int fx, fy;
    while((x < baseGame->getBaseGrid()->getWidth() && y < baseGame->getBaseGrid()->getWidth()) && (x >= 0 && y >= 0))
    {
        searchAlign(x--, y++, win, alc, fx, fy);
    }
    //Logger::log << "checkColumnAlign: " << alc << std::endl;
}
void WinnerChecker::checkRowAlign(int y)
{
    if(getWinnerId() != -1)
        return;
    if(this->filledCells < this->minCells)
        return;

    int win = -1;
    int alc = 0;
    int fx, fy;
    for (int x = 0; x < baseGame->getBaseGrid()->getWidth(); ++x)
    {
        searchAlign(x, y, win, alc, fx, fy);
    }
    //Logger::log << "checkRowAlign: " << alc << std::endl;
}

void WinnerChecker::searchAlign(int x, int y, int &win, int &alc, int &fx, int &fy)
{
    int ccell = baseGame->getBaseGrid()->getGridAt(x, y);
    if(ccell == -1)
        return;
    if(t != NULL)
        t[x][y]++;
    if(win != ccell || ccell <= 0)
    {
        win = ccell;
        alc = 1;
        fx = x;
        fy = y;
    }
    else
    {
        alc++;
        if(alc == baseGame->getGameSettings()->getAlignSize())
        {
            Logger::log << "Align found for " << win << std::endl;
            this->winAlignements[win - 1][winAlignementsCount[win - 1]++].set(win - 1, fx, fy, x, y);
            alc = 0;
            win = -1;
        }
    }
}

void WinnerChecker::onPlaceToken(int x, int y)
{
    Logger::log << "WinnerChecker::onPlaceToken: " << x << "," << y << std::endl;
    this->filledCells++;

    /*resetWinAlignementsCount();

    checkColumnAlign(x);
    checkRowAlign(y);

    updateFlags();*/
    onRotate();
}
void WinnerChecker::onRemoveToken(int x, int y)
{
    Logger::log << "WinnerChecker::onRemoveToken: " << x << "," << y << std::endl;
    this->filledCells--;

    /*resetWinAlignementsCount();

    for (int i = 0; i <= y; ++i)
    {
        checkRowAlign(i);
    }
    checkColumnAlign(x);

    updateFlags();*/
    onRotate();
}
void WinnerChecker::onRotate()
{
    Logger::log << "WinnerChecker::onRotate" <<  std::endl;

    resetWinAlignementsCount();

    for (int i = 0; i < baseGame->getBaseGrid()->getWidth(); ++i)
    {
        checkColumnAlign(i);
    }
    for (int i = 0; i < baseGame->getBaseGrid()->getHeight(); ++i)
    {
        checkRowAlign(i);
    }
    t = new int*[baseGame->getBaseGrid()->getWidth()];
    for (int i = 0; i < baseGame->getBaseGrid()->getWidth(); ++i)
    {
        t[i] = new int[baseGame->getBaseGrid()->getHeight()];
        for (int j = 0; j < baseGame->getBaseGrid()->getHeight(); ++j)
        {
            t[i][j] = 0;
        }
    }

    checkDiagonalAlign();

    for (int i = 0; i < baseGame->getBaseGrid()->getWidth(); ++i)
    {
        for (int j = 0; j < baseGame->getBaseGrid()->getHeight(); ++j)
        {
            Logger::log << t[i][j] << " ";
        }
        Logger::log << std::endl;
    }

    for (int i = 0; i < baseGame->getBaseGrid()->getWidth(); ++i)
        delete  [] t[i];
    delete [] t;
    t = NULL;

    updateFlags();
}

int WinnerChecker::getWinnerId()
{
    for(int i = 0; i < baseGame->getGameSettings()->getNumPlayers(); i++)
        if(this->winner[i])
            return i;
    return -1;
}
TokenAlignement *WinnerChecker::getWinAlignement(int playerId)
{
    return this->winAlignements[playerId];
}
bool WinnerChecker::hasWinner()
{
    return winnerFlag;
}
bool WinnerChecker::hasDraw()
{
    return drawFlag;
}