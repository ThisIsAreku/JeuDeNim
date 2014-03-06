#ifndef _GRID_H_
#define _GRID_H_

#include "BaseGrid.h"
#include "display/Renderable.h"

#include "GravityProvider.h"
#include "TokenAnimator.h"

class Grid :  public BaseGrid, public Renderable
{
    int width;
    int height;

    int totalCells;
    int filledCells;

    int last_x;
    int last_y;

    int **matrice;

    int currentRotationValue;

    GravityProvider *gravityProvider;
    TokenAnimator *tokenAnimator;

    void convertCoords(int &, int &);

    bool forceSetGridAt(int, int, int);

    void drawRuler();
    void drawBaseGrid();
    void drawTokens();
    void clearGridArea();

    void redrawAll();

public:
    Grid(WindowManager *, int, int, int);
    virtual ~Grid();

    int getGridAt(int, int);
    bool setGridAt(int, int, int);
    int getWidth();
    int getHeight();

    bool placeToken(int, int);
    bool removeToken(int, int);
    void moveToken(int, int, int, int);
    void rotate(EntityTurnAction r);

    void init();
    UpdateState update(chtype);
    void render();


    bool isEmpty();
    bool isFull();

    int getShiftX();
    int getShiftY();

    void setShiftX(int);
    void setShiftY(int);

};

#endif
