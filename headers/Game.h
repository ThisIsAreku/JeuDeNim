#ifndef _GAME_H_
#define _GAME_H_

#include <string>

#include "display/WindowManager.h"
#include "Grid.h"

class Game
{
    Grid *grid;
    WindowManager *manager;

public:
    Game();
    ~Game();
    void displayGrid();
    void wait();

};

#endif
