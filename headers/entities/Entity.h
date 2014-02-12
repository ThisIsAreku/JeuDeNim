#ifndef _ENTITIY_H_
#define _ENTITIY_H_

#include <string>

#include "Game.h"

class Entity
{
    Game &game;

protected:
    Entity(Game &);
    ~Entity();

public:
    virtual int turn();
    virtual std::string getId();
};

#endif
