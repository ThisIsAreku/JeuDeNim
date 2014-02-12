#ifndef _RENDERABLE_H_
#define _RENDERABLE_H_

#include "display/Window.h"

class Renderable
{
    Window *win;
protected:
    Renderable(Window *);
    virtual ~Renderable();

    Window* getWindow();
public:

    virtual void init() = 0;
    virtual void update() = 0;
    virtual void render() = 0;

};

#endif