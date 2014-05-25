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
public:
    Overlay(WindowManager *);
    virtual ~Overlay();

    bool isVisible();
};

#endif
