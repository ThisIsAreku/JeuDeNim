#include <iostream>
#include "Grille.h"

Grille::Grille(int width, int height)
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

Grille::~Grille()
{
    for (int i = 0; i < this->width; ++i)
    {
        delete [] this->matrice[i];
    }
    delete [] this->matrice;
}