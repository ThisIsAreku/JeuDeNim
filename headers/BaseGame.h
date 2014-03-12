#ifndef _BASE_GAME_H_
#define _BASE_GAME_H_

#include "BaseGrid.h"
#include "config/GameSettings.h"
#include "providers/GravityProvider.h"
#include "display/WindowManager.h"

class Entity; // forward declaration
#include "entities/Entity.h"
#include "entities/EntityListener.h"

class WinnerChecker; // forward declaration
#include "WinnerChecker.h"

class BaseGame : public EntityListener
{
public:
    virtual void start() = 0;
    virtual WindowManager *getWindowManager() = 0;
    virtual Entity *getCurrentPlayer() = 0;
    virtual BaseGrid *getBaseGrid() = 0;
    virtual GameSettings *getGameSettings() = 0;
    virtual GravityProvider *getGravityProvider() = 0;
    virtual WinnerChecker *getWinnerChecker() = 0;

    virtual void doKeyboardActions(chtype) = 0;

    virtual bool onEntityTurnCompleted(EntityTurnAction, int, int) = 0;

};

#endif
