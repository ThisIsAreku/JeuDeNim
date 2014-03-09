#include "providers/GravityProvider.h"

GravityProvider::GravityProvider(BaseGrid *grid)
{
    this->grid = grid;
}
GravityProvider::~GravityProvider()
{
}

BaseGrid *GravityProvider::getBaseGrid()
{
    return this->grid;
}