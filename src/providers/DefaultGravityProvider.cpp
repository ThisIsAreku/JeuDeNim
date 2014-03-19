#include <iostream>
#include "providers/DefaultGravityProvider.h"

#include "Logger.h"

DefaultGravityProvider::DefaultGravityProvider(Grid *grid) : GravityProvider(grid)
{
}

void DefaultGravityProvider::doGravity(std::function<void(int, int, int, int)> callback)
{
    for(int i = 0; i < getGrid()->getWidth(); ++i)
    {
        doColumnGravity(i, callback);
    }
}

int DefaultGravityProvider::findNewTokenY(int x, int y, int max = -1)
{
    if(y >= getGrid()->getHeight() - 1)
        return y;
    if(max == -1)
        max = getGrid()->getHeight();
    int rv = y;
    for(int i = y + 1; i < max; ++i)
    {
        if(getGrid()->getGridAt(x, i) > 0)
        {
            return i - 1;
        }
        rv = i;
    }
    return rv;
}

int DefaultGravityProvider::findFirstEmptyCell(int x)
{
    for(int i = 0; i < getGrid()->getHeight(); ++i)
    {
        if(getGrid()->getGridAt(x, i) > 0)
            return i - 1;
    }
    return getGrid()->getHeight() - 1;
}


void DefaultGravityProvider::doColumnGravity(int x, std::function<void(int, int, int, int)> callback)
{
    int player, newY;
    int lower = getGrid()->getHeight();
    for(int j = getGrid()->getHeight() - 1; j >= 0; --j)
    {
        player = getGrid()->getGridAt(x, j);
        if(player > 0)
        {
            newY = findNewTokenY(x, j, lower);
            lower--;
            if(newY != j)
            {
                getGrid()->moveGridAt(x, j, x, newY);
                if(callback != NULL)
                    callback(player, x, j, newY);
            }
        }
    }
}