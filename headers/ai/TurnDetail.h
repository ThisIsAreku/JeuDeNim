#ifndef _TURN_DETAIL_H_
#define _TURN_DETAIL_H_

#include "structs/EntityTurnAction.h"

struct TurnDetail
{
    EntityTurnAction action;
    int data_x;
    int data_y;

    TurnDetail &operator =(TurnDetail b)
    {
        action = b.action;
        data_x = b.data_x;
        data_y = b.data_y;
        return (*this);
    }

    TurnDetail() : action(TOKEN_PLACE), data_x(0), data_y(0) {}

    TurnDetail(EntityTurnAction act, int x, int y)
    {
        action = act;
        data_x = x;
        data_y = y;
    }
};

#endif