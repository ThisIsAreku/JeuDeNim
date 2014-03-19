#ifndef _AI_H_
#define _AI_H_

#include "entities/Entity.h"
#include "entities/EntityListener.h"

class AI : public Entity
{
public:
    AI(Game *, int);
    virtual ~AI();

    int turn();

    void init();
    UpdateState update(int);
    void render();

    const char *getId();
    int getEntityType();
};

#endif
