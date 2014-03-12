#ifndef _HUMAIN_H_
#define _HUMAIN_H_

#include "entities/Entity.h"
#include "entities/EntityListener.h"

#include "widgets/CellCursor.h"

class Humain : public Entity
{
    CellCursor *cursor;
    EntityTurnAction turnAction;

    bool onCursorSelect(int, int);
public:
    Humain(BaseGame *, int);
    virtual ~Humain();

    int turn();

    void init();
    UpdateState update(int);
    void render();

    const char *getId();
    int getEntityType();
};

#endif
