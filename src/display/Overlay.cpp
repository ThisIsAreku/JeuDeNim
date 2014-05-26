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
	if(isVisible()){
		Logger::log << "Overlay::close" << std::endl;
		getWindowManager()->clearOverlay();
		visible = false;
	}
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

	Logger::log << "Overlay::update" << std::endl;

	if(static_cast<int>(ch) == 27){
		close();
		return SUCCESS;
	}

	return updateOverlay(ch);
}

void Overlay::render()
{
	if(!isVisible())
		return;
    
	Logger::log << "Overlay::render" << std::endl;
    renderOverlay();
    getWindow()->refresh();
    overlay(stdscr, getWindow()->getHandle());
    refresh();
}