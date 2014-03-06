#include <sstream>

#include "widgets/Menu.h"


Menu::Menu(WindowManager *manager, int winId, int (BaseGame::*callback)(int)) : Renderable(manager, winId)
{
    this->callback = callback;
}
Menu::~Menu()
{
}
void Menu::init()
{
}
UpdateState Menu::update(chtype)
{
    return SUCCESS;
}
void Menu::render()
{
}
