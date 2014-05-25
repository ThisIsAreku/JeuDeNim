#include <iostream>
#include "providers/DefaultGravityProvider.h"

#include "Grid.h"
#include "Logger.h"
#include "Helpers.h"

DefaultGravityProvider::DefaultGravityProvider(Grid *grid) : super(grid)
{
    int maxVal = Helpers::__max(grid->getWidth(), grid->getHeight());
    lowerCellForColumn = new int[maxVal];
    std::fill(this->lowerCellForColumn, this->lowerCellForColumn + maxVal, grid->getHeight() - 1);
}
DefaultGravityProvider::~DefaultGravityProvider()
{
    delete [] lowerCellForColumn;
}

void DefaultGravityProvider::doGravity(std::function<void(int, int, int, int)> callback)
{
    for(int i = 0; i < getGrid()->getWidth(); ++i)
    {
        doColumnGravity(i, callback);
    }
    seekAllFirstEmptyCell();
}

int DefaultGravityProvider::findNewTokenY(const int &x, const int &y, int max = -1) const
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

int DefaultGravityProvider::getFirstEmptyCell(const int &x) const
{
    return this->lowerCellForColumn[x];
}



void DefaultGravityProvider::doColumnGravity(const int &x, std::function<void(int, int, int, int)> callback)
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

void DefaultGravityProvider::changeColumnFirstEmptyCell(const int &x, const int &v)
{
    lowerCellForColumn[x] += v;
}
void DefaultGravityProvider::seekAllFirstEmptyCell()
{
    std::fill(this->lowerCellForColumn, this->lowerCellForColumn + Helpers::__max(getGrid()->getWidth(), getGrid()->getHeight()), getGrid()->getHeight() - 1);
    for (int x = 0; x < getGrid()->getWidth(); ++x)
    {
        for(int i = 0; i < getGrid()->getHeight(); ++i)
        {
            if(getGrid()->getGridAt(x, i) > 0)
            {
                this->lowerCellForColumn[x] = i - 1;
                break;
            }
        }
    }
}
