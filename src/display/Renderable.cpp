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

void Renderable::log(const char *c)
{
    if(this->manager == NULL)
        return;
    while(!this->manager->append(WIN_SCOREBOARD, c))
        this->manager->clearWindow(WIN_SCOREBOARD);
    this->manager->refreshWindow(WIN_SCOREBOARD);
}
void Renderable::log(int n)
{
    if(this->manager == NULL)
        return;
    while(!this->manager->appendInt(WIN_SCOREBOARD, n))
        this->manager->clearWindow(WIN_SCOREBOARD);
    this->manager->refreshWindow(WIN_SCOREBOARD);
}