#ifndef _GAME_SETTINGS_H_
#define _GAME_SETTINGS_H_

class GameSettings
{
    int numPlayers;

    int boardWidth;
    int boardHeight;

    int numAlign;
    int alignSize;

    bool commited;
public:
    GameSettings();

    GameSettings *setNumPlayers(int);

    GameSettings *setBoardWidth(int);
    GameSettings *setBoardHeight(int);

    GameSettings *setNumAlign(int);
    GameSettings *setAlignSize(int);

    int getNumPlayers();

    int getBoardWidth();
    int getBoardHeight();

    int getNumAlign();
    int getAlignSize();


    void commit();
};

#endif