#include <iostream>
#include "Grid.h"

Grid::Grid(int width, int height)
{
    this->width = width;
    this->height = height;
    this->matrice = new int*[width];
    for (int i = 0; i < width; ++i)
    {
        this->matrice[i] = new int[height];
        for (int j = 0; j < height; ++j)
        {
            this->matrice[i][j] = -1;
        }
    }
}

Grid::~Grid()
{
    for (int i = 0; i < this->width; ++i)
    {
        delete [] this->matrice[i];
    }
    delete [] this->matrice;
}


int Grid::getGridAt(int &x, int &y)
{
    if((0 < x || x < width) || (0 < y || y < height))
        return -1;
    return this->matrice[x][y];
}

void Grid::setGridAt(int &x, int &y, int &v)
{
    if((0 < x || x < width) || (0 < y || y < height))
        return;
    this->matrice[x][y] = v;
}


int &Grid::getWidth()
{
    return this->width;
}
int &Grid::getHeight()
{
    return this->height;
}