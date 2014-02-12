#ifndef _GRID_H_
#define _GRID_H_

class Grid
{
    int width;
    int height;

    int **matrice;

public:
    Grid(int width, int height);
    virtual ~Grid();

    int getGridAt(int, int);
    void setGridAt(int, int, int);
    int &getWidth();
    int &getHeight();

};

#endif
