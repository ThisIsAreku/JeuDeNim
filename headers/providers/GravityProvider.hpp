#ifndef _GRAVITY_PROVIDER_H_
#define _GRAVITY_PROVIDER_H_

class Grid;
#include <functional>   // std::bind

class GravityProvider
{
    Grid *grid;
protected:
    Grid *getGrid() const;
public:
    GravityProvider(Grid *);
    virtual ~GravityProvider();

    virtual void doGravity(std::function<void(int, int, int, int)>) = 0;
    virtual void doColumnGravity(const int &, std::function<void(int, int, int, int)>) = 0;
    virtual int getFirstEmptyCell(const int &) const = 0;

    virtual void changeColumnFirstEmptyCell(const int &, const int &) = 0;
    virtual void seekAllFirstEmptyCell() = 0;


};

#endif
