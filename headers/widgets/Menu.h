#ifndef _CELL_CURSOR_H_
#define _CELL_CURSOR_H_

#include "display/Renderable.h"

class Menu : public Renderable
{
    int (BaseGame::*callback)(int) = NULL;
    int x;
    int y;
    int prev_x;
    int prev_y;

    int max_x;
    int max_y;

    void drawBox(int x, int y);
    void drawBox();
public:
    Menu(WindowManager *, int, int (BaseGame::*callback)(int));
    virtual ~Menu();

    void init();
    UpdateState update(chtype);
    void render();

};

#endif


