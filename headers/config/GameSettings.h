#ifndef _GAME_SETTINGS_H_
#define _GAME_SETTINGS_H_

#include "display/Window.h"

class GameSettings
{
    int numPlayers;

    int boardWidth;
    int boardHeight;

    int numAlign;
    int alignSize;

    int playerTypes[4];
    int aiLevel[4];

    bool commited;

    void askForProperty(Window *, int, const char * , int &, int, int);
public:
    GameSettings();

    GameSettings *setNumPlayers(int);

    GameSettings *setBoardWidth(int);
    GameSettings *setBoardHeight(int);

    GameSettings *setNumAlign(int);
    GameSettings *setAlignSize(int);

    GameSettings *setPlayerType(int, int);

    int getNumPlayers() const;

    int getBoardWidth() const;
    int getBoardHeight() const;

    int getNumAlign() const;
    int getAlignSize() const;

    int *getPlayerTypes();
    int *getAILevels();

    bool animate;


    GameSettings *input(Window *, int);
    void commit();
};

#endif