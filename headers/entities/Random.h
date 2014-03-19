#ifndef _RANDOM_H_
#define _RANDOM_H_

#include "entities/Entity.h"
#include "entities/EntityListener.h"

class Random : public Entity
{
    bool doChoice();
public:
    Random(Game *, int);
    virtual ~Random();

    int turn();

    void init();
    UpdateState update(int);
    void render();

    const char *getId();
    int getEntityType();
};

#endif
