#include "overlays/SaveOverlay.hpp"
#include "Game.hpp"
#include "display/Window.hpp"

SaveOverlay::SaveOverlay(Game *game) : super(game->getWindowManager())
{
	this->game = game;
}
SaveOverlay::~SaveOverlay()
{
}

void SaveOverlay::initOverlay()
{
	action = 0;
	askSlot = false;
}

UpdateState SaveOverlay::updateOverlay(chtype ch)
{
	if(ch == KEY_LEFT && action > 0)
		action--;
	else if(ch ==KEY_RIGHT && action < 1)
		action++;
	else if(ch == 10)
	{
		askSlot = true;
	}
	else if(action == -1)
	    close();
    return SUCCESS;
}

void SaveOverlay::renderOverlay()
{
    Window *win = getWindow();
    if(win == NULL)
        return;

    win->clear();

    if(action == 0)
    	win->AttribOn(COLOR_PAIR(30));
    win->printAt(10, 2, "SAVEGARDER");
    win->AttribOff(COLOR_PAIR(30));
    if(action == 1)
    	win->AttribOn(COLOR_PAIR(30));
    win->printAt(-17, 2, "CHARGER");
    win->AttribOff(COLOR_PAIR(30));

    if(askSlot)
    {
    	int slot = 0;
	    win->printAt(2, 4, "Slot : 0");
	    win->readAnyAt(9, 4, "%d", &slot);
	    close();
	    close();
	    if(action == 0)
	    	this->game->saveState(slot);
	    else if(action == 1)
	    	this->game->restoreState(slot);
	    action = -1;
	    ungetch(-1);
    }
}