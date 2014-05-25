#ifndef _STRUCT_ENTITY_TURN_ACTION_
#define _STRUCT_ENTITY_TURN_ACTION_

enum EntityTurnAction
{
    TOKEN_PLACE,
    TOKEN_REMOVE,
    ROTATE
};
enum Rotation
{
    ROTATE_CLOCKWISE = 1,
    ROTATE_COUNTERCLOCKWISE = -1
};

#endif
