#include "GravityProvider.h"

GravityProvider::GravityProvider(BaseGrid *grid)
{
    _grid = grid;
}

void GravityProvider::doGravity(std::function<void(int, int, int, int)> callback)
{
    for(int i = 0; i < _grid->getWidth(); ++i)
    {
        doColumnGravity(i, callback);
    }
}

int GravityProvider::findNewTokenY(int x, int y)
{
    if(y >= _grid->getHeight() - 1)
        return y;
    int rv = y;
    for(int i = y + 1; i < _grid->getHeight(); ++i)
    {
        if(_grid->getGridAt(x, i) > 0)
        {
            return i - 1;
        }
        rv = i;
    }
    return rv;
}

int GravityProvider::findFirstEmptyCell(int x)
{
    for(int i = 0; i < _grid->getHeight(); ++i)
    {
        if(_grid->getGridAt(x, i) > 0)
            return i - 1;
    }
    return _grid->getHeight() - 1;
}


void GravityProvider::doColumnGravity(int x, std::function<void(int, int, int, int)> callback)
{
    int player, newY;
    for(int j = _grid->getHeight() - 1; j >= 0; --j)
    {
        player = _grid->getGridAt(x, j);
        if(player > 0)
        {
            newY = findNewTokenY(x, j);
            if(newY != j)
            {
                _grid->moveToken(x, j, x, newY);
                if(callback != NULL)
                    callback(player, x, j, newY);
            }
        }
    }
}