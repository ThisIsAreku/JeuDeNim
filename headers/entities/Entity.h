#ifndef _ENTITIY_H_
#define _ENTITIY_H_

#include <cstring>

#include "Game.h"

class Entity
{
    Game &game;

protected:
    Entity(Game &);
    virtual ~Entity();

public:
    virtual int turn() = 0;
    virtual std::string getId() = 0;
};

#endif
