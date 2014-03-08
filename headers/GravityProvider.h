#ifndef _GRAVITY_PROVIDER_H_
#define _GRAVITY_PROVIDER_H_

#include "BaseGrid.h"
#include <functional>   // std::bind

class GravityProvider
{
    BaseGrid *_grid;
public:
    GravityProvider(BaseGrid *);
    void doGravity(std::function<void(int, int, int, int)>);
    int findNewTokenY(int, int, int);
    int findFirstEmptyCell(int);

    void doColumnGravity(int, std::function<void(int, int, int, int)>);

};

#endif