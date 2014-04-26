#include "config/GameSettings.h"
#include "constants.h"

GameSettings::GameSettings()
{
    numPlayers = 2;

    boardWidth = 7;
    boardHeight = 6;

    numAlign = 1;
    alignSize = 4;

    commited = false;

    animate = true;

    for(int i = 0; i < 4; i++)
        playerTypes[i] = 0;
    for(int i = 0; i < 4; i++)
        aiLevel[i] = 3;
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
GameSettings *GameSettings::setPlayerType(int playerId, int playerType)
{
    if(!this->commited)
        this->playerTypes[playerId] = playerType;
    return this;
}
GameSettings *GameSettings::setAILevel(int playerId, int aiLevel)
{
    if(!this->commited)
        this->aiLevel[playerId] = aiLevel;
    return this;
}

int GameSettings::getNumPlayers() const
{
    return numPlayers;
}
int GameSettings::getBoardWidth() const
{
    return boardWidth;
}
int GameSettings::getBoardHeight() const
{
    return boardHeight;
}
int GameSettings::getNumAlign() const
{
    return numAlign;
}
int GameSettings::getAlignSize() const
{
    return alignSize;
}
int *GameSettings::getPlayerTypes()
{
    return playerTypes;
}
int *GameSettings::getAILevels()
{
    return aiLevel;
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
    win->printAt(0, baseLine++, "Types de joueurs disponibles: ");
    win->printAt(0, baseLine++, "0: Humain, 1: Débile, 2: IA");
    win->printAt(0, baseLine++, "(IA n'est pas encore implémenté)");

    char msg[19] = "Type du joueur X: ";
    char msg_lvl[44] = "Difficulté (0: Ghââ!?.., 15: hardcore): ";
    for(int i = 0; i < numPlayers; i++)
    {
        msg[15] = '0' + static_cast<char>(i + 1);
        askForProperty(win, baseLine++, msg, playerTypes[i], 0, 2);
        if(playerTypes[i] == ENTITY_AI)
            askForProperty(win, baseLine++, msg_lvl, aiLevel[i], 0, 15);

    }
    return this;
}


void GameSettings::askForProperty(Window *win, int line, const char *title, int &prop, int min, int max)
{
    win->printAt(0, line, title);
    char str[10];
    sprintf(str, "%d", prop);
    win->printAt(static_cast<int>(strlen(title)), line, str);
    win->refresh();
    do
    {
        win->readAnyAt(static_cast<int>(strlen(title)), line, "%d", &prop);
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



std::ostream &operator <<(std::ostream &a, GameSettings &b)
{
    a << b.alignSize << " " << b.numAlign << std::endl
      << b.boardWidth << " " << b.boardHeight << std::endl;

    a << b.numPlayers << std::endl;
    for (int i = 0; i < b.numPlayers; ++i)
    {
        a << b.playerTypes[i] << " ";
    }
    a << std::endl;
    for (int i = 0; i < b.numPlayers; ++i)
    {
        a << b.aiLevel[i] << " ";
    }
    a << std::endl;
    return a;
}

std::istream &operator >>(std::istream &a, GameSettings &b)
{
    a >> b.alignSize >> b.numAlign >> b.boardWidth >> b.boardHeight >> b.numPlayers;
    for (int i = 0; i < b.numPlayers; ++i)
    {
        a >> b.playerTypes[i];
    }
    for (int i = 0; i < b.numPlayers; ++i)
    {
        a >> b.aiLevel[i];
    }
    b.commit();
    return a;
}
