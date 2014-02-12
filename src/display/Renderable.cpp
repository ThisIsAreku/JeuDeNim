#include "display/Renderable.h"

Renderable::Renderable(Window* win)
{
	this->win = win;
}
Window* Renderable::getWindow()
{
	return this->win;
}