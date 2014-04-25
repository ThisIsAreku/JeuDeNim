#ifndef _GRID_H_
#define _GRID_H_

#include <fstream>

#include "structs/EntityTurnAction.h"
#include "providers/GravityProvider.h"
class GravityProvider;

class Grid
{

    GravityProvider *gravityProvider;

    bool modified;

    int currentRotationValue;

    int initWidth, initHeight;

    int filledCells, totalCells;

    int **matrice;
    int *matriceData;
    int *countCellsByPlayers;

    void convertCoords(int &, int &) const;

    void allocate();
public:
    Grid(int, int);
    Grid(Grid &);
    ~Grid();
    int getGridAt(int, int) const;
    bool setGridAt(int, int, int);
    bool forceSetGridAt(int, int, int);
    void moveGridAt(int, int, int, int);

    int getWidth() const;
    int getHeight() const;

    void rotate(int);

    bool isEmpty() const;
    bool isFull() const;
    int getFilledCells() const;
    int getTotalCells() const;
    int getCellsForPlayer(const int &) const;
    int getRotation() const;


    void debugGrid() const;

    void clone(Grid &b);

    bool isModified() const;
    void resetModified();


    GravityProvider *getGravityProvider();
    Grid &operator =(Grid);

    friend std::ostream &operator <<(std::ostream &, Grid &);
    friend std::istream &operator >>(std::istream &, Grid &);

};


#endif