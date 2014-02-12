#include "display/Window.h"

Window::Window(int winId, int w, int h, int x, int y)
{
    this->width = w;
    this->height = h;
    this->x = x;
    this->y = y;
    this->id = winId;
    handle = newwin(h, w, y, x);
    box(handle, 0 , 0);
    wrefresh(handle);
}
Window::~Window()
{
    wborder(handle, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
    wrefresh(handle);
    delwin(handle);
}

void Window::refresh()
{
    wrefresh(handle);
}
bool Window::printAt(int x, int y, const char *str)
{
    if((x < 1 || width - 2 < x) || (y < 1 || height - 2 < y))
        return false;
    mvwprintw(handle, x, y, str);
    this->refresh();
    return true;
}
bool Window::readAt(int x, int y, const char *str)
{
    curs_set(1);
    echo();
    int r = mvwscanw(handle, x, y, "%s", str);
    noecho();
    curs_set(0);
    return r;

}

int Window::getWidth()
{
    return this->width;
}

int Window::getHeight()
{
    return this->height;
}

int Window::getX()
{
    return this->x;
}

int Window::getY()
{
    return this->y;
}
int Window::getId()
{
    return this->id;
}