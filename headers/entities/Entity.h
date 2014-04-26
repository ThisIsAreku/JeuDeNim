#ifndef _ENTITIY_H_
#define _ENTITIY_H_

class Game; // forward declaration

#include "Game.h"

#include "structs/UpdateState.h"

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

    virtual double getOperationPercent() const = 0;
};

#endif
