#include "display/Overlay.hpp"
#include "display/WindowManager.hpp"
#include "display/Window.hpp"

#include "constants.hpp"
#include "Logger.hpp"

Overlay::Overlay(WindowManager *manager) : super(manager, WIN_OVERLAY)
{
	this->visible = false;
	//init();
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
void Overlay::close()
{
	if(isVisible())
		getWindowManager()->clearOverlay();
}

void Overlay::init()
{
	if(isVisible())
		initOverlay();
}

UpdateState Overlay::update(chtype ch)
{
	if(!isVisible())
		return SUCCESS;

	if(static_cast<int>(ch) == 27){
		Logger::log << static_cast<int>(ch) << "hh hhh" << std::endl;
		close();
		return SUCCESS;
	}

	return updateOverlay(ch);
}

void Overlay::render()
{
	if(isVisible())
    	renderOverlay();
    getWindow()->refresh();
    overlay(stdscr, getWindow()->getHandle());
    refresh();
}