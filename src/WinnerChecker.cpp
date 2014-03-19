#include "WinnerChecker.h"



#include "Logger.h"

WinnerChecker::WinnerChecker(Game *game, bool useGraphics)
{
    this->useGraphics = useGraphics;
    this->game = game;
    this->totalCells = game->getGrid()->getWidth() * game->getGrid()->getHeight();
    this->filledCells = 0;
    //nombre minimal de jetons pour un alignement
    this->minCells = (game->getGameSettings()->getAlignSize() * game->getGameSettings()->getNumAlign()) - (game->getGameSettings()->getNumAlign() - 1);
    this->winner = new bool[game->getGameSettings()->getNumPlayers()];
    resetWinner();

    winAlignementsCount = new int[game->getGameSettings()->getNumPlayers()];

    if(useGraphics)
    {
        winAlignements = new TokenAlignement*[game->getGameSettings()->getNumPlayers()];
        for (int i = 0; i < game->getGameSettings()->getNumPlayers(); ++i)
            winAlignements[i] = new TokenAlignement[game->getGameSettings()->getAlignSize()];
    }
}
WinnerChecker::~WinnerChecker()
{
    delete [] this->winner;
    if(useGraphics)
    {
        for (int i = 0; i < game->getGameSettings()->getNumPlayers(); ++i)
        {
            delete [] winAlignements[i];
        }
        delete [] winAlignements;
    }
}

void WinnerChecker::resetWinAlignementsCount()
{
    for(int i = 0; i < game->getGameSettings()->getNumPlayers(); i++)
        winAlignementsCount[i] = 0;
}

void WinnerChecker::resetWinner()
{
    for(int i = 0; i < game->getGameSettings()->getNumPlayers(); i++)
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
    for (int i = 0; i < game->getGameSettings()->getNumPlayers(); ++i)
    {
        if(winAlignementsCount[i] == game->getGameSettings()->getNumAlign())
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
    if(nbrWinner == game->getGameSettings()->getNumPlayers())
        drawFlag = true;
    else
        drawFlag = false;
}

void WinnerChecker::checkColumnAlign(int x)
{
    if(hasWinner())
        return;
    if(this->filledCells < this->minCells)
        return;

    int win = -1;
    int alc = 0;
    int fx(0), fy(0);
    for (int y = 0; y < game->getGrid()->getHeight(); ++y)
    {
        searchAlign(x, y, win, alc, fx, fy);
    }
    //Logger::log << "checkColumnAlign: " << alc << std::endl;
}
void WinnerChecker::checkDiagonalAlign()
{
    if(hasWinner())
        return;
    if(this->filledCells < this->minCells)
        return;

    for (int v = 0; v < game->getGrid()->getWidth(); ++v)
    {
        checkDiagonal1Align(v, 0);
        checkDiagonal2Align(v, 0);
    }
    for (int v = 0; v < game->getGrid()->getHeight(); ++v)
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
    int fx(0), fy(0);
    Logger::log << "checkDiagonal1Align: " << x << ", " << y << std::endl;
    while((x < game->getGrid()->getWidth() && y < game->getGrid()->getWidth()) && (x >= 0 && y >= 0))
    {
        searchAlign(x++, y++, win, alc, fx, fy);
    }
}
void WinnerChecker::checkDiagonal2Align(int x, int y) // r->l
{
    int win = -1;
    int alc = 0;
    int fx(0), fy(0);
    Logger::log << "checkDiagonal2Align: " << x << ", " << y << std::endl;
    while((x < game->getGrid()->getWidth() && y < game->getGrid()->getWidth()) && (x >= 0 && y >= 0))
    {
        searchAlign(x--, y++, win, alc, fx, fy);
    }
}
void WinnerChecker::checkRowAlign(int y)
{
    if(hasWinner())
        return;
    if(this->filledCells < this->minCells)
        return;

    int win = -1;
    int alc = 0;
    int fx(0), fy(0);
    for (int x = 0; x < game->getGrid()->getWidth(); ++x)
    {
        searchAlign(x, y, win, alc, fx, fy);
    }
    //Logger::log << "checkRowAlign: " << alc << std::endl;
}

void WinnerChecker::searchAlign(int x, int y, int &win, int &alc, int &fx, int &fy)
{
    int ccell = game->getGrid()->getGridAt(x, y);
    if(ccell == -1)
        return;

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
        if(alc == game->getGameSettings()->getAlignSize())
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

    for (int i = 0; i < game->getGrid()->getWidth(); ++i)
    {
        checkColumnAlign(i);
    }
    for (int i = 0; i < game->getGrid()->getHeight(); ++i)
    {
        checkRowAlign(i);
    }

    Logger::log << "Diagonals.." << std::endl;
    checkDiagonalAlign();

    updateFlags();
}

int WinnerChecker::getWinnerId(int id)
{
    int n = 0;
    for(int i = 0; i < game->getGameSettings()->getNumPlayers(); i++)
        if(this->winner[i])
            if(n++ == id)
                return i;
    return -1;
}
int WinnerChecker::getNumWinner()
{
    int n = 0;
    for(int i = 0; i < game->getGameSettings()->getNumPlayers(); i++)
        if(this->winner[i])
            n++;
    return n;
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