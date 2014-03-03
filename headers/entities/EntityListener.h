#ifndef _ENTITIY_LISTENER_H_
#define _ENTITIY_LISTENER_H_

#include "structs/EntityTurnAction.h"

class EntityListener
{
public:
    virtual bool onEntityTurnCompleted(EntityTurnAction, int, int) = 0;
};

#endif
