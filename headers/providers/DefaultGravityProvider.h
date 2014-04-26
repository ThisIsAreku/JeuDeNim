#ifndef _DEFAULT_GRAVITY_PROVIDER_H_
#define _DEFAULT_GRAVITY_PROVIDER_H_

#include "providers/GravityProvider.h"

class DefaultGravityProvider : public GravityProvider
{
    int findNewTokenY(const int &, const int &, int) const;
    int *lowerCellForColumn;
public:
    DefaultGravityProvider(Grid *grid);
    ~DefaultGravityProvider();

    void doGravity(std::function<void(int, int, int, int)>);
    void doColumnGravity(const int &, std::function<void(int, int, int, int)>);
    int getFirstEmptyCell(const int &) const;

    void changeColumnFirstEmptyCell(const int &, const int &);
    void seekAllFirstEmptyCell();

};

#endif
