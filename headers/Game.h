#ifndef _GAME_H_
#define _GAME_H_

#include "Game.h"

#include "config/GameSettings.h"

#include "display/WindowManager.h"

#include "widgets/DisplayGrid.h"
class DisplayGrid;
#include "widgets/TokenLiner.h"
class TokenLiner;

#include "entities/Entity.h"
class Entity;

#include "providers/GravityProvider.h"

#include "WinnerChecker.h"
class WinnerChecker;
#include "Grid.h"

#include <iostream>
#include <fstream>

class Game
{
    Grid *grid;
    DisplayGrid *displayGrid;
    WindowManager *manager;
    GameSettings *gameSettings;
    GravityProvider *gravityProvider;
    WinnerChecker *winnerChecker;

    TokenLiner *tokenLiner;

    int playTurnIndex;

    Entity **players;
    int currentPlayer;

    bool interrupted;
    bool game_end;
    bool turn_end;

    bool random_testing;

    bool displayingHelp;

    unsigned long tick;

    const unsigned long konami[10] {259, 259, 258, 258, 260, 261, 260, 261, 98, 97};
    int konamiStep;

    char *saveFileName;
    char *saveFilePath;

    void createEntities();

    void loop();

    void init();
    void render();
    void update();
    void deinit();

    void appendToPlayTurns(const char *);
    void logKeyboard(chtype);

public:
    Game();
    virtual ~Game();

    //static long getTime();

    void start();
    WindowManager *getWindowManager();
    Entity *getCurrentPlayer();
    DisplayGrid *getDisplayGrid();
    Grid *getGrid();
    GameSettings *getGameSettings();
    WinnerChecker *getWinnerChecker();

    void doKeyboardActions(chtype);

    void invokeEntityTurn(int);

    void renderOps();

    void playerWin();

    void displayHelp();

    bool onEntityTurnCompleted(EntityTurnAction, int, int);

    bool isRandomTesting();
    void setRandomTesting(bool);

    void saveState(int);
    void restoreState(int);

    void reset();

};

#endif
