#ifndef _HUMAIN_H_
#define _HUMAIN_H_

#include "entities/Entity.hpp"
#include "structs/EntityTurnAction.hpp"

class CellCursor;

class Humain : public Entity
{
    typedef Entity super;
    CellCursor *cursor;
    EntityTurnAction turnAction;

    bool onCursorSelect(int, int);
public:
    Humain(Game *, int);
    virtual ~Humain();

    int turn();

    void init();
    UpdateState update(chtype);
    void render();

    const char *getId();
    int getEntityType();
};

#endif
