#ifndef _HUMAIN_H_
#define _HUMAIN_H_

#include <string>

#include "Game.h"

class Humain : public Entity
{
    Game &game;

public:
    Humain(Game &);
    ~Humain();

    int turn();
    std::string getId();
};

#endif
