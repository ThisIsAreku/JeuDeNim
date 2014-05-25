#include "display/Overlay.h"

Overlay::Overlay(WindowManager *manager) : super(manager, WIN_OVERLAY)
{
}
Overlay::~Overlay()
{

}
Window *Overlay::getWindow()
{
    return super::getWindow();
}
bool Overlay::isVisible()
{
    return this->visible;
}
