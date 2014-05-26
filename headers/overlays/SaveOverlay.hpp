#ifndef _SAVE_OVERLAY_H_
#define _SAVE_OVERLAY_H_

#include "display/Overlay.hpp"

class Game;

class SaveOverlay : public Overlay
{
    typedef Overlay super;

    Game *game;
    int action;
    bool askSlot;
public:
    SaveOverlay(Game *);
    virtual ~SaveOverlay();

    void initOverlay();
    UpdateState updateOverlay(chtype);
    void renderOverlay();
};

#endif
