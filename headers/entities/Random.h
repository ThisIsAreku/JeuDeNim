#ifndef _RANDOM_H_
#define _RANDOM_H_

#include "entities/Entity.h"

class Random : public Entity
{
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

    double getOperationPercent() const
    {
        return .0;
    };
};

#endif
