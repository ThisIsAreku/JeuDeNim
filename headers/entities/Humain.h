#ifndef _HUMAIN_H_
#define _HUMAIN_H_

#include <cstring>

#include "Game.h"

class Humain : public Entity
{
    Game &game;

public:
    Humain(Game &);
    virtual ~Humain();

    int turn();
    std::string getId();
};

#endif
