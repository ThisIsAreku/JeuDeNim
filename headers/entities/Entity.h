#ifndef _ENTITIY_H_
#define _ENTITIY_H_

class BaseGame; // forward declaration

#include "BaseGame.h"

#include "structs/UpdateState.h"

class Entity
{
    BaseGame *game;
    int entityIndex;

protected:
    Entity(BaseGame *, int);
    BaseGame *getBaseGame();

public:
    virtual ~Entity() = 0;

    int getEntityIndex();

    virtual int turn() = 0;

    virtual void init() = 0;
    virtual UpdateState update(int) = 0;
    virtual void render() = 0;

    virtual const char *getId() = 0;
    virtual int getEntityType() = 0;
};

#endif
