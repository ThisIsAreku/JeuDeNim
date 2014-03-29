#ifndef _GRID_H_
#define _GRID_H_

#include "structs/EntityTurnAction.h"

class Grid
{

    int currentRotationValue;

    int initWidth, initHeight;

    int filledCells, totalCells;

    int last_x, last_y;

    int **matrice;

    void convertCoords(int &, int &);

public:
    Grid(int, int);
    ~Grid();
    int getGridAt(int, int);
    bool setGridAt(int, int, int);
    bool forceSetGridAt(int, int, int);
    void moveGridAt(int, int, int, int);

    int getWidth();
    int getHeight();

    void rotate(int);

    bool isEmpty();
    bool isFull();


    void logGridDebug();


};

#endif