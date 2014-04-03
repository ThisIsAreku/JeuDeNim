#include "WinnerChecker.h"



#include "Logger.h"

WinnerChecker::WinnerChecker(Game *_game, bool _useGraphics)
{
    useGraphics = _useGraphics;
    game = _game;
    totalCells = game->getGrid()->getWidth() * game->getGrid()->getHeight();
    filledCells = 0;
    //nombre minimal de jetons pour un alignement
    minCells = (game->getGameSettings()->getAlignSize() * game->getGameSettings()->getNumAlign()) - (game->getGameSettings()->getNumAlign() - 1);
    winner = new bool[game->getGameSettings()->getNumPlayers()];
    resetWinner();

    winAlignementsCount = new int[game->getGameSettings()->getNumPlayers()];

    if(useGraphics)
    {
        winAlignements = new TokenAlignement*[game->getGameSettings()->getNumPlayers()];
        for (int i = 0; i < game->getGameSettings()->getNumPlayers(); ++i)
            winAlignements[i] = new TokenAlignement[game->getGameSettings()->getAlignSize()];
    }

    diagonalDebug = NULL;
    diagonalDebugStep = 0;
    diagonalDebugW = 0;
}
WinnerChecker::~WinnerChecker()
{
    delete [] this->winner;
    delete [] winAlignementsCount;

    if(useGraphics)
    {
        for (int i = 0; i < game->getGameSettings()->getNumPlayers(); ++i)
        {
            delete [] winAlignements[i];
        }
        delete [] winAlignements;
    }



    if(Logger::log.isDebugEnabled())
    {
        if(diagonalDebug != NULL)
        {
            for (int d = 0; d < 2; ++d)
            {
                for (int i = 0; i < diagonalDebugW; ++i)
                {
                    delete [] diagonalDebug[d][i];
                }
                delete [] diagonalDebug[d];
            }
            delete [] diagonalDebug;
        }
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
        Logger::log << "winAlignementsCount for " << i << " : " << winAlignementsCount[i] << std::endl;
        if(winAlignementsCount[i] >= game->getGameSettings()->getNumAlign())
        {
            Logger::log << i << " win" << std::endl;
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
void WinnerChecker::logDiagonalDebug()
{
    if(diagonalDebug == NULL)
        return;
    Logger::log << "== Diagonals Debug ==" << std::endl;
    for (int d = 0; d <= diagonalDebugStep; ++d)
    {
        Logger::log << "Step " << d << std::endl;
        for (int i = 0; i < game->getGrid()->getWidth(); ++i)
        {
            for (int j = 0; j < game->getGrid()->getHeight(); ++j)
            {
                Logger::log << diagonalDebug[d][i][j] << " ";
            }
            Logger::log << std::endl;
        }
    }
}
void WinnerChecker::checkDiagonalAlign()
{
    if(hasWinner())
        return;
    if(this->filledCells < this->minCells)
        return;

    /*if(Logger::log.isDebugEnabled()){
        diagonalDebug = new int**[2];
        diagonalDebugW = game->getGrid()->getWidth();
        diagonalDebugH = game->getGrid()->getHeight();
        for (int d = 0; d < 2; ++d)
        {
            diagonalDebug[d] = new int*[diagonalDebugW];
            for (int i = 0; i < diagonalDebugW; ++i)
            {
                diagonalDebug[d][i] = new int[diagonalDebugH];
                for (int j = 0; j < diagonalDebugH; ++j)
                {
                    diagonalDebug[d][i][j] = 0;
                }
            }
        }
    }*/

    Logger::log << "Diagonal on width" << std::endl;
    diagonalDebugStep = 0;
    unsigned int x = game->getGrid()->getWidth() - 1;
    for (int v = 0; v < game->getGrid()->getWidth(); ++v)
    {
        checkDiagonal1Align(v, 0);
        checkDiagonal2Align(v, 0);
    }

    for (int v = 1; v < game->getGrid()->getHeight(); ++v)
    {
        checkDiagonal1Align(0, v);
        checkDiagonal2Align(x, v);
    }

    /*if(Logger::log.isDebugEnabled()){
        logDiagonalDebug();
        for (int d = 0; d <= diagonalDebugStep; ++d)
        {
            for (int i = 0; i < game->getGrid()->getWidth(); ++i)
            {
                delete [] diagonalDebug[d][i];
            }
            delete [] diagonalDebug[d];
        }
        delete [] diagonalDebug;
        diagonalDebug = NULL;
        Logger::log << "Grid is" << std::endl;
        game->getGrid()->logGridDebug();
    }*/
    //Logger::log << "checkColumnAlign: " << alc << std::endl;
}
void WinnerChecker::checkDiagonal1Align(int x, int y) // tl->br
{
    int win = -1;
    int alc = 0;
    int fx(0), fy(0);
    //Logger::log << "checkDiagonal1Align: " << x << ", " << y << std::endl;
    while((x < game->getGrid()->getWidth() && y < game->getGrid()->getHeight()) && (x >= 0 && y >= 0))
    {
        searchAlign(x++, y++, win, alc, fx, fy);
    }
}
void WinnerChecker::checkDiagonal2Align(int x, int y) // tr->bl
{
    int win = -1;
    int alc = 0;
    int fx(0), fy(0);
    //Logger::log << "checkDiagonal2Align: " << x << ", " << y << std::endl;
    while((x < game->getGrid()->getWidth() && y < game->getGrid()->getHeight()) && (x >= 0 && y >= 0))
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

    if(Logger::log.isDebugEnabled())
    {
        if(diagonalDebug != NULL)
        {
            diagonalDebug[diagonalDebugStep][x][y]++;
            logDiagonalDebug();
        }
    }

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
    this->filledCells++;
    if(this->filledCells < this->minCells)
        return;

    Logger::log << "WinnerChecker::onPlaceToken: " << x << "," << y << std::endl;

    /*resetWinAlignementsCount();

    checkColumnAlign(x);
    checkRowAlign(y);

    updateFlags();*/
    onRotate();
}
void WinnerChecker::onRemoveToken(int x, int y)
{
    this->filledCells--;
    if(this->filledCells < this->minCells)
        return;

    Logger::log << "WinnerChecker::onRemoveToken: " << x << "," << y << std::endl;

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
    if(this->filledCells < this->minCells)
        return;

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