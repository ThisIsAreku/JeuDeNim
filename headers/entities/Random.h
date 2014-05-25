#ifndef _RANDOM_H_
#define _RANDOM_H_

#include "entities/Entity.h"
#include "structs/EntityTurnAction.h"

class Game;

class Random : public Entity
{
    typedef Entity super;
    bool doChoice();
public:
    Random(Game *, int);
    virtual ~Random();

    int turn();

    void init();
    UpdateState update(chtype);
    void render();

    const char *getId();
    int getEntityType();
};

#endif
