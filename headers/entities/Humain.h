#ifndef _HUMAIN_H_
#define _HUMAIN_H_

#include "entities/Entity.h"

#include "widgets/CellCursor.h"

class Humain : public Entity
{
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

    double getOperationPercent() const
    {
        return .0;
    };
};

#endif
