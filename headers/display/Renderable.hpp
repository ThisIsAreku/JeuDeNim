#ifndef _RENDERABLE_H_
#define _RENDERABLE_H_

#include <ncurses.h>

class WindowManager;
class Window;

#include "structs/UpdateState.hpp"

class Renderable
{
    int winId;
    WindowManager *manager;

protected:
    Renderable(WindowManager *, int);

    Window *getWindow();
    WindowManager *getWindowManager();
public:
    virtual ~Renderable() = 0;

    virtual void init() = 0;
    virtual UpdateState update(chtype) = 0;
    virtual void render() = 0;

};

#endif
