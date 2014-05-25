#ifndef _ENTITIY_H_
#define _ENTITIY_H_

#include <ncurses.h>
#include "structs/UpdateState.hpp"

class Game;

class Entity
{
    Game *game;
    int entityIndex;

protected:
    Entity(Game *, int);
    Game *getGame() const;

public:
    virtual ~Entity() = 0;

    int getEntityIndex() const;

    virtual int turn() = 0;

    virtual void init() = 0;
    virtual UpdateState update(chtype) = 0;
    virtual void render() = 0;

    virtual const char *getId() = 0;
    virtual int getEntityType() = 0;
};

#endif
