#ifndef _RENDERABLE_H_
#define _RENDERABLE_H_

#include "display/WindowManager.h"
#include "BaseGame.h"

#include "structs/UpdateState.h"

class Renderable
{
    int winId;
    WindowManager *manager;

protected:
    Renderable(WindowManager *, int);

    Window *getWindow();
public:
    virtual ~Renderable() = 0;

    virtual void init() = 0;
    virtual UpdateState update(chtype) = 0;
    virtual void render() = 0;

};

#endif