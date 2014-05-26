#include "display/Renderable.hpp"

#include "display/WindowManager.hpp"

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
WindowManager *Renderable::getWindowManager()
{
    if(this->manager == NULL)
        return NULL;
       return this->manager;
}
