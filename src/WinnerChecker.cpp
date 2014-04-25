#include "WinnerChecker.h"



#include "Logger.h"

WinnerChecker::WinnerChecker(Game *_game, bool _useGraphics)
{
    useGraphics = _useGraphics;
    game = _game;
    grid = _game->getGrid();

    //nombre minimal de jetons pour un alignement
    minCells = (game->getGameSettings()->getAlignSize() * game->getGameSettings()->getNumAlign()) - (game->getGameSettings()->getNumAlign() - 1);
    winner = new bool[game->getGameSettings()->getNumPlayers()];
    resetWinner();

    winAlignementsCount = new int[game->getGameSettings()->getNumPlayers()];
    alignementsSizes = new int[game->getGameSettings()->getNumPlayers()];

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
    delete [] winAlignementsCount;
    delete [] alignementsSizes;

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
    std::fill(alignementsSizes, alignementsSizes + game->getGameSettings()->getNumPlayers(), 0);
    std::fill(winAlignementsCount, winAlignementsCount + game->getGameSettings()->getNumPlayers(), 0);
}

void WinnerChecker::resetWinner()
{
    std::fill(winner, winner + game->getGameSettings()->getNumPlayers(), false);

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
        //Logger::log << "winAlignementsCount for " << i << " : " << winAlignementsCount[i] << std::endl;
        if(winAlignementsCount[i] >= game->getGameSettings()->getNumAlign())
        {
            //Logger::log << "WinnerChecker::updateFlags: " << i << " win" << std::endl;
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
    if(grid->getFilledCells() < this->minCells)
        return;

    int win = -1;
    int alc = 0;
    int fx(0), fy(0);
    for (int y = 0; y < grid->getHeight(); ++y)
    {
        searchAlign(x, y, win, alc, fx, fy);
    }
}
void WinnerChecker::checkDiagonalAlign()
{
    if(hasWinner())
        return;
    if(grid->getFilledCells() < this->minCells)
        return;

    //Logger::log << "checkDiagonalAlign: Width" << std::endl;
    unsigned int x = grid->getWidth() - 1;
    for (int v = 0; v < grid->getWidth(); ++v)
    {
        checkDiagonal1Align(v, 0);
        checkDiagonal2Align(v, 0);
    }

    //Logger::log << "checkDiagonalAlign: Height" << std::endl;
    for (int v = 1; v < grid->getHeight(); ++v)
    {
        checkDiagonal1Align(0, v);
        checkDiagonal2Align(x, v);
    }
}
void WinnerChecker::checkDiagonal1Align(int x, int y) // tl->br
{
    int win = -1;
    int alc = 0;
    int fx(0), fy(0);
    while((x < grid->getWidth() && y < grid->getHeight()) && (x >= 0 && y >= 0))
    {
        searchAlign(x++, y++, win, alc, fx, fy);
    }
}
void WinnerChecker::checkDiagonal2Align(int x, int y) // tr->bl
{
    int win = -1;
    int alc = 0;
    int fx(0), fy(0);
    while((x < grid->getWidth() && y < grid->getHeight()) && (x >= 0 && y >= 0))
    {
        searchAlign(x--, y++, win, alc, fx, fy);
    }
}
void WinnerChecker::checkRowAlign(int y)
{
    if(hasWinner())
        return;
    if(grid->getFilledCells() < this->minCells)
        return;

    int win = -1;
    int alc = 0;
    int fx(0), fy(0);
    for (int x = 0; x < grid->getWidth(); ++x)
    {
        searchAlign(x, y, win, alc, fx, fy);
    }
}

void WinnerChecker::searchAlign(int x, int y, int &win, int &alc, int &fx, int &fy)
{
    int ccell = grid->getGridAt(x, y);
    if(ccell == -1)
    {
        Logger::log << "-1 for " << x << "," << y << std::endl;
        return;
    }

    if(win != ccell || ccell <= 0)
    {
        win = ccell;
        alc = 1;
        fx = x;
        fy = y;
        if(win > 0)
        {
            //Logger::log << "win: " << win << ", alc: " << alc << ", this->alignementsSizes[win - 1]: " << this->alignementsSizes[win - 1] << std::endl;
            if(this->alignementsSizes[win - 1] < alc)
                this->alignementsSizes[win - 1] = alc;
        }
    }
    else
    {
        alc++;
        //Logger::log << "win: " << win << ", alc: " << alc << ", this->alignementsSizes[win - 1]: " << this->alignementsSizes[win - 1] << std::endl;
        if(this->alignementsSizes[win - 1] < alc)
            this->alignementsSizes[win - 1] = alc;
        if(alc == game->getGameSettings()->getAlignSize())
        {
            //Logger::log << "Align found for " << win << std::endl;
            if(useGraphics)
                this->winAlignements[win - 1][winAlignementsCount[win - 1]++].set(win - 1, fx, fy, x, y);
            else
                winAlignementsCount[win - 1]++;
            alc = 0;
            win = -1;
        }
    }
}

void WinnerChecker::setGridToCheck(const Grid *grid)
{
    if(this->grid == grid)
        return;
    if(grid == NULL)
        return;
    //Logger::log << "WinnerChecker: Old grid is " << this->grid << ", new grid is " << grid << std::endl;
    resetWinAlignementsCount();
    this->grid = grid;
}

void WinnerChecker::searchWinner(Grid *grid, bool forceCheck)
{
    //Logger::log << "WinnerChecker::searchWinner" <<  std::endl;
    if(grid != NULL)
        setGridToCheck(grid);

    if(!forceCheck && (grid->getFilledCells() < this->minCells))
    {
        Logger::log << "WinnerChecker::searchWinner skipped" << std::endl;
        return;
    }
    //grid->debugGrid();


    //Logger::log << "WinnerChecker::searchWinner start" <<  std::endl;
    resetWinAlignementsCount();

    //Logger::log << "WinnerChecker::searchWinner checkColumnAlign" <<  std::endl;
    for (int i = 0; i < grid->getWidth(); ++i)
    {
        checkColumnAlign(i);
    }
    //Logger::log << "WinnerChecker::searchWinner checkRowAlign" <<  std::endl;
    for (int i = 0; i < grid->getHeight(); ++i)
    {
        checkRowAlign(i);
    }

    //Logger::log << "WinnerChecker::searchWinner checkDiagonalAlign" <<  std::endl;
    checkDiagonalAlign();

    updateFlags();

    grid->resetModified();
}

bool WinnerChecker::fastWinCheck(Grid *grid, bool forceCheck)
{
    searchWinner(grid, forceCheck);
    return winnerFlag;
}

int WinnerChecker::getWinnerId(int id) const
{
    int n = 0;
    for(int i = 0; i < game->getGameSettings()->getNumPlayers(); i++)
        if(this->winner[i])
            if(n++ == id)
                return i;
    return -1;
}
bool WinnerChecker::isWinner(int id) const
{
    return this->winner[id];
}
int WinnerChecker::getNumWinner() const
{
    int n = 0;
    for(int i = 0; i < game->getGameSettings()->getNumPlayers(); i++)
        if(this->winner[i])
            n++;
    return n;
}
int WinnerChecker::getMaxAlignSize(int playerId) const
{
    return this->alignementsSizes[playerId];
}
TokenAlignement *WinnerChecker::getWinAlignement(int playerId) const
{
    if(!useGraphics)
        return NULL;
    return this->winAlignements[playerId];
}
int WinnerChecker::getNumWinAlignements(int playerId) const
{
    return this->winAlignementsCount[playerId];
}
bool WinnerChecker::hasWinner() const
{
    return winnerFlag;
}
bool WinnerChecker::hasDraw() const
{
    return drawFlag;
}