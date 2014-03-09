#ifndef _DEFAULT_GRAVITY_PROVIDER_H_
#define _DEFAULT_GRAVITY_PROVIDER_H_

#include "providers/GravityProvider.h"

class DefaultGravityProvider : public GravityProvider
{
    int findNewTokenY(int, int, int);
public:
    DefaultGravityProvider(BaseGrid *grid);

    void doGravity(std::function<void(int, int, int, int)>);
    void doColumnGravity(int, std::function<void(int, int, int, int)>);

    int findFirstEmptyCell(int);


};

#endif