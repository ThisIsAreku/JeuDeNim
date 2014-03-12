#ifndef _GAME_H_
#define _GAME_H_

#include "BaseGame.h"
#include <list>

#include "display/Renderable.h"

#include "widgets/Grid.h"

class Game : public BaseGame
{
    Grid *grid;
    WindowManager *manager;
    GameSettings *gameSettings;
    GravityProvider *gravityProvider;
    WinnerChecker *winnerChecker;

    int playTurnIndex;

    Entity **players;
    int currentPlayer;

    bool interrupted;
    bool game_end;

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

    static long getTime();

    void start();
    WindowManager *getWindowManager();
    Entity *getCurrentPlayer();
    BaseGrid *getBaseGrid();
    GameSettings *getGameSettings();
    GravityProvider *getGravityProvider();
    WinnerChecker *getWinnerChecker();

    void doKeyboardActions(chtype);

    void invokeEntityTurn(int);

    void playerWin();


    bool onEntityTurnCompleted(EntityTurnAction, int, int);

};

#endif
