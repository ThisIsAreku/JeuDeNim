#ifndef _BASE_GRID_H_
#define _BASE_GRID_H_

#include "structs/EntityTurnAction.h"

class BaseGrid
{
public:
    virtual int getGridAt(int, int) = 0;
    virtual bool setGridAt(int, int, int) = 0;
    virtual int getWidth() = 0;
    virtual int getHeight() = 0;

    virtual bool isDoingSteppedGravity() = 0;
    virtual void rotate(EntityTurnAction r) = 0;

    virtual bool placeToken(int, int) = 0;
    virtual bool removeToken(int, int) = 0;
    virtual void moveToken(int, int, int, int) = 0;

    virtual bool isEmpty() = 0;
    virtual bool isFull() = 0;
};

#endif