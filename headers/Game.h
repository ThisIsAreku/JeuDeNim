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

    int playTurnIndex;

    Entity **players;
    int currentPlayer;
    int numberOfPlayers;

    int initGridW, initGridH;

    bool interrupted;

    void loop();

    void init();
    void render();
    void update();
    void deinit();

    void appendToPlayTurns(const char *);

public:
    Game();
    virtual ~Game();

    static long getTime();

    void start();
    WindowManager *getWindowManager();
    Entity *getCurrentPlayer();
    BaseGrid *getBaseGrid();
    int getNumberOfPlayers();

    void invokeEntityTurn(int);


    bool onEntityTurnCompleted(EntityTurnAction, int, int);

};

#endif
