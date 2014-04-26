#include "display/Renderable.h"

Renderable::Renderable(WindowManager *manager, int winId)
{
    this->winId = winId;
    this->manager = manager;
}
Renderable::~Renderable()
{

}
Window *Renderable::getWindow()
{
    if(this->manager == NULL)
        return NULL;
    return this->manager->getWindow(winId);
}
