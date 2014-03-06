#include "display/Window.h"

Window::Window(int winId, int w, int h, int x, int y)
{
    this->width = w;
    this->height = h;
    this->x = x;
    this->y = y;
    this->id = winId;
    this->pos_x = this->pos_y = 0;
    this->shift_x = this->shift_y = 0;
    handle = NULL;
    handle = newwin(h, w, y, x);
    keypad(handle, TRUE);
    box(handle, 0 , 0);
    wrefresh(handle);
}
Window::~Window()
{
    wborder(handle, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
    wrefresh(handle);
    delwin(handle);
    handle = NULL;
}

void Window::applyShift(int &x, int &y)
{
    x += shift_x;
    y += shift_y;
}
bool Window::contains(int x, int y)
{
    return (x < 1 || width - 2 < x) || (y < 1 || height - 2 < y);
}

void Window::clear()
{
    if(handle == NULL)
        return;
    wclear(handle);
    pos_x = pos_y = 0;

}
void Window::refresh()
{
    if(handle == NULL)
        return;
    box(handle, 0 , 0);
    wrefresh(handle);
}
bool Window::printAt(int x, int y, const char *str)
{
    if(handle == NULL)
        return false;
    x++; //skip border
    y++; //again
    applyShift(x, y);
    if(contains(x, y))
        return false;
    mvwprintw(handle, y, x, str);
    //this->refresh();
    return true;
}
bool Window::printAt_unshifted(int x, int y, const char *str)
{
    if(handle == NULL)
        return false;
    x++; //skip border
    y++; //again
    if(contains(x, y))
        return false;
    mvwprintw(handle, y, x, str);
    //this->refresh();
    return true;
}
void Window::setChar(int x, int y, chtype ch)
{
    if(handle == NULL)
        return;
    x++; //skip border
    y++; //again
    applyShift(x, y);
    if((x < 1 || width - 2 < x) || (y < 1 || height - 2 < y))
        return;
    mvwaddch(handle, y, x, ch);
}

bool Window::readAt(int x, int y, const char *str)
{
    if(handle == NULL)
        return false;
    x++; //skip border
    y++; //again
    applyShift(x, y);
    if(contains(x, y))
        return false;
    curs_set(1);
    echo();
    int r = mvwscanw(handle, y, x, "%s", str);
    noecho();
    curs_set(0);
    return r;
}
bool Window::readAnyAt(int x, int y, const char *format, const void *str)
{
    if(handle == NULL)
        return false;
    x++; //skip border
    y++; //again
    applyShift(x, y);
    if(contains(x, y))
        return false;
    curs_set(1);
    echo();
    int r = mvwscanw(handle, y, x, format, str);
    noecho();
    curs_set(0);
    return r;
}

bool Window::append(const char *c)
{
    if(handle == NULL)
        return false;
    int len = strlen(c);
    if(pos_x + len > width - 2)
    {
        if(!newLine())
            return false;
    }
    bool rslt = printAt(pos_x, pos_y, c);
    pos_x += len;
    return rslt;
}
bool Window::newLine()
{
    pos_x = 0;
    pos_y++;
    if(pos_y > height - 2)
        return false;
    return true;
}

void Window::AttribOn(int attr)
{
    if(handle == NULL)
        return;
    wattron(handle, attr);
}
void Window::AttribOff(int attr)
{
    if(handle == NULL)
        return;
    wattroff(handle, attr);
}
void Window::AttribSet(int attr)
{
    if(handle == NULL)
        return;
    wattrset(handle, attr);
}
void Window::AttribResetOff()
{
    if(handle == NULL)
        return;
    wattrset(handle, A_NORMAL);
    wstandend(handle);
}
void Window::AttribResetOn()
{
    if(handle == NULL)
        return;
    wstandout(handle);
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


int Window::getShiftX()
{
    return shift_x;
}

int Window::getShiftY()
{
    return shift_y;
}

void Window::setShiftX(int x)
{
    shift_x = x;
}

void Window::setShiftY(int y)
{
    shift_y = y;
}