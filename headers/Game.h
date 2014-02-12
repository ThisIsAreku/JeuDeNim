#ifndef _GAME_H_
#define _GAME_H_

#include <cstring>

#include "display/WindowManager.h"
#include "Grid.h"

class Game
{
    Grid *grid;
    WindowManager *manager;

    bool interrupted;

    void loop();

    void init();
    void render();
    void update();

public:
    Game();
    virtual ~Game();
    void displayGrid();
    void wait();
    void start();

};

#endif
