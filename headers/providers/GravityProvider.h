#ifndef _GRAVITY_PROVIDER_H_
#define _GRAVITY_PROVIDER_H_

#include "BaseGrid.h"
#include <functional>   // std::bind

class GravityProvider
{
    BaseGrid *grid;
protected:
    BaseGrid *getBaseGrid();
public:
    GravityProvider(BaseGrid *);
    virtual ~GravityProvider();

    virtual void doGravity(std::function<void(int, int, int, int)>) = 0;
    virtual void doColumnGravity(int, std::function<void(int, int, int, int)>) = 0;

    virtual int findFirstEmptyCell(int) = 0;


};

#endif