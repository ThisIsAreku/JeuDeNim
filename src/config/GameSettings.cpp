#include "config/GameSettings.h"

GameSettings::GameSettings()
{
    numPlayers = 2;

    boardWidth = 7;
    boardHeight = 6;

    numAlign = 1;
    alignSize = 4;

    commited = false;
}

GameSettings *GameSettings::setNumPlayers(int numPlayers)
{
    if(!this->commited)
        this->numPlayers = numPlayers;
    return this;
}
GameSettings *GameSettings::setBoardWidth(int boardWidth)
{
    if(!this->commited)
        this->boardWidth = boardWidth;
    return this;
}
GameSettings *GameSettings::setBoardHeight(int boardHeight)
{
    if(!this->commited)
        this->boardHeight = boardHeight;
    return this;
}
GameSettings *GameSettings::setNumAlign(int numAlign)
{
    if(!this->commited)
        this->numAlign = numAlign;
    return this;
}
GameSettings *GameSettings::setAlignSize(int alignSize)
{
    if(!this->commited)
        this->alignSize = alignSize;
    return this;
}

int GameSettings::getNumPlayers()
{
    return numPlayers;
}
int GameSettings::getBoardWidth()
{
    return boardWidth;
}
int GameSettings::getBoardHeight()
{
    return boardHeight;
}
int GameSettings::getNumAlign()
{
    return numAlign;
}
int GameSettings::getAlignSize()
{
    return alignSize;
}

void GameSettings::commit()
{
    this->commited = true;
}