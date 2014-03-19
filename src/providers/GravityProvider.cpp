#include "providers/GravityProvider.h"

GravityProvider::GravityProvider(Grid *grid)
{
    this->grid = grid;
}
GravityProvider::~GravityProvider()
{
}

Grid *GravityProvider::getGrid()
{
    return this->grid;
}