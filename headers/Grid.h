#ifndef _GRID_H_
#define _GRID_H_

class Grid
{
    int width;
    int height;

    int **matrice;

public:
    Grid(int width, int height);
    ~Grid();

};

#endif
