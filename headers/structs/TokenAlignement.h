#ifndef _STRUCT_TOKEN_ALIGNEMENT_
#define _STRUCT_TOKEN_ALIGNEMENT_

#include "Helpers.h"

#include "Logger.h"

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
    void set(int _playerId, int _x1, int _y1, int _x2, int _y2)
    {
        Logger::log << "TokenAlignement set(" << _playerId << ", " << _x1 << ", " << _y1 << ", " << _x2 << ", " << _y2 << ")" << std::endl;
        playerId = _playerId;
        if(_x1 == _x2)
        {
            orientation = VERTICAL;
            if(_y1 > _y2)
            {
                //Helpers::__switch_vars(_y1, _y2);
                x1 = _x1;
                y1 = _y2;
                x2 = _x2;
                y2 = _y1;
            }
            else
            {
                x1 = _x1;
                y1 = _y1;
                x2 = _x2;
                y2 = _y2;
            }
        }
        else if(_y1 == _y2)
        {
            orientation = HORIZONTAL;
            if(_x1 > _x2)
            {
                //Helpers::__switch_vars(_x1, _x2);
                x1 = _x2;
                y1 = _y1;
                x2 = _x1;
                y2 = _y2;
            }
            else
            {
                x1 = _x1;
                y1 = _y1;
                x2 = _x2;
                y2 = _y2;
            }
        }
        else
        {
            orientation = DIAGONAL;
            x1 = _x1;
            y1 = _y1;
            x2 = _x2;
            y2 = _y2;
        }

        /*
                x1 = _x1;
                y1 = _y1;
                x2 = _x2;
                y2 = _y2;
        */
    };
};

#endif