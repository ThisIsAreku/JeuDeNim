#include "config/GameSettings.h"

GameSettings::GameSettings()
{
    numPlayers = 2;

    boardWidth = 7;
    boardHeight = 6;

    numAlign = 1;
    alignSize = 4;

    commited = false;

    animate = true;
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

GameSettings *GameSettings::input(Window *win, int baseLine)
{
    askForProperty(win, baseLine++, "Entrez la taille des alignements: ", alignSize, 2, 10);
    askForProperty(win, baseLine++, "Entrez le nombre d'alignements: ", numAlign, 1, 4);
    askForProperty(win, baseLine++, "Entrez la largeur de la grille: ", boardWidth, alignSize, 40);
    askForProperty(win, baseLine++, "Entrez la longueur de la grille: ", boardHeight, alignSize, 40);
    askForProperty(win, baseLine++, "Entrez le nombre de joueurs (2-4): ", numPlayers, 2, 4);
    return this;
}


void GameSettings::askForProperty(Window *win, int line, const char *title, int &prop, int min, int max)
{
    win->printAt(0, line, title);
    win->refresh();
    do
    {
        win->readAnyAt(strlen(title), line, "%d", &prop);
        if(prop < min || max < prop)
        {
            win->AttribOn(COLOR_PAIR(30));
            win->printAt(0, line + 1, "Valeur incorrecte");
            win->AttribOff(COLOR_PAIR(30));
            win->refresh();
        }
    }
    while(prop < min || max < prop);
}