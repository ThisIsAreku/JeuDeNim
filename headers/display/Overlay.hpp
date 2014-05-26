#ifndef _OVERLAY_H_
#define _OVERLAY_H_

#include "display/Renderable.hpp"

class Overlay : public Renderable
{
    friend class WindowManager;
    typedef Renderable super;

    bool visible;

protected:
    Window *getWindow();
    void close();
public:
    Overlay(WindowManager *);
    virtual ~Overlay();

    bool isVisible();

    void init();
    UpdateState update(chtype);
    void render();

    virtual void initOverlay() = 0;
    virtual UpdateState updateOverlay(chtype) = 0;
    virtual void renderOverlay() = 0;
};

#endif
