#ifndef _STRUCT_TOKEN_ALIGNEMENT_
#define _STRUCT_TOKEN_ALIGNEMENT_

#include "Helpers.h"

enum AlignOrientation
{
    HORIZONTAL,
    VERTICAL,
    DIAGONAL,
    ANY
};

struct TokenAlignement
{
    int playerId;
    int x1, y1;
    int x2, y2;
    AlignOrientation orientation;
    bool operator ==(TokenAlignement b)
    {
        return (b.x1 == x1 && b.y1 == y1) && (b.x2 == x2 && b.y2 == y2) && (b.orientation == orientation || b.orientation == ANY);
    };
    void set(int playerId, int x1, int y1, int x2, int y2)
    {
        this->playerId = playerId;
        if(x1 == x2)
        {
            orientation = VERTICAL;
            if(y1 > y2)
                Helpers::__switch_vars(y1, y2);
        }
        else if(y1 == y2)
        {
            orientation = HORIZONTAL;
            if(x1 > x2)
                Helpers::__switch_vars(x1, x2);
        }
        else
            orientation = DIAGONAL;


        this->x1 = x1;
        this->y1 = y1;
        this->x2 = x2;
        this->y2 = y2;
    };
};

#endif