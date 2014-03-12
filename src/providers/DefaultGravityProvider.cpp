#include <iostream>
#include "providers/DefaultGravityProvider.h"

DefaultGravityProvider::DefaultGravityProvider(BaseGrid *grid) : GravityProvider(grid)
{
}

void DefaultGravityProvider::doGravity(std::function<void(int, int, int, int)> callback)
{
    for(int i = 0; i < getBaseGrid()->getWidth(); ++i)
    {
        doColumnGravity(i, callback);
    }
}

int DefaultGravityProvider::findNewTokenY(int x, int y, int max = -1)
{
    if(y >= getBaseGrid()->getHeight() - 1)
        return y;
    if(max == -1)
        max = getBaseGrid()->getHeight();
    int rv = y;
    for(int i = y + 1; i < max; ++i)
    {
        if(getBaseGrid()->getGridAt(x, i) > 0)
        {
            return i - 1;
        }
        rv = i;
    }
    return rv;
}

int DefaultGravityProvider::findFirstEmptyCell(int x)
{
    for(int i = 0; i < getBaseGrid()->getHeight(); ++i)
    {
        if(getBaseGrid()->getGridAt(x, i) > 0)
            return i - 1;
    }
    return getBaseGrid()->getHeight() - 1;
}


void DefaultGravityProvider::doColumnGravity(int x, std::function<void(int, int, int, int)> callback)
{
    int player, newY;
    int lower = getBaseGrid()->getHeight();
    for(int j = getBaseGrid()->getHeight() - 1; j >= 0; --j)
    {
        player = getBaseGrid()->getGridAt(x, j);
        if(player > 0)
        {
            newY = findNewTokenY(x, j, lower);
            lower--;
            if(newY != j)
            {
                getBaseGrid()->moveToken(x, j, x, newY);
                if(callback != NULL)
                    callback(player, x, j, newY);
            }
        }
    }
}