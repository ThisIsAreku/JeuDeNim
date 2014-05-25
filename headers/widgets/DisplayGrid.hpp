#ifndef _DISPLAY_GRID_H_
#define _DISPLAY_GRID_H_

#include "display/Renderable.hpp"

#include "structs/EntityTurnAction.hpp"

class TokenAnimator;
class Grid;
class Game;

class DisplayGrid : public Renderable
{
    typedef Renderable super;
    Game *game;

    bool shiftEnabled;

    TokenAnimator *tokenAnimator;

    void drawRuler();
    void drawBaseGrid();
    void drawTokens();
    void clearGridArea();

    void redrawAll(bool);


    int getWidth();
    int getHeight();

public:
    DisplayGrid(WindowManager *, int, Game *);
    virtual ~DisplayGrid();

    bool placeToken(int, int);
    bool removeToken(int, int);
    void moveToken(int, int, int, int);
    void rotate(Rotation r);

    void init();
    UpdateState update(chtype);
    void render();

    int getShiftX();
    int getShiftY();

    void setShiftX(int);
    void setShiftY(int);

};

#endif
