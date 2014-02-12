#include "display/WindowManager.h"

WindowManager::WindowManager()
{
    this->win = new Window*[WIN_COUNT];
    for(int i = 0; i < WIN_COUNT; i++)
        this->win[i] = NULL;
    //this->grille = new Grille(10, 10);
    initNcurses();
    initWindows();

}
WindowManager::~WindowManager()
{
    releaseNcurses();
    delete [] this->win;
}

/************/
/* PRIVATES */
/************/
void WindowManager::createWindow(int winId, int w, int h, int x, int y)
{
    if(winId >= WIN_COUNT)
        return;
    if(this->win[winId] != NULL)
        return;
    this->win[winId] = new Window(winId, w, h, x, y);
}
void WindowManager::deleteWindow(int winId)
{
    if(winId >= WIN_COUNT)
        return;
    if(this->win[winId] == NULL)
        return;
    delete this->win[winId];
    this->win[winId] = NULL;
}
Window *WindowManager::getWindow(int winId)
{
    if(winId >= WIN_COUNT)
        return NULL;
    if(this->win[winId] == NULL)
        initWindows();
    return this->win[winId];
}
void WindowManager::initWindows()
{

    int cols = COLS / 3;
    int lines = LINES - 5;

    createWindow(WIN_GAME_GRID,     2 * cols,   lines,  0,              0);
    createWindow(WIN_SCOREBOARD,    cols,       lines,  2 * cols + 1,   0);
    createWindow(WIN_GAME_TURN,     COLS - 1,   5,      0,              lines);
}
void WindowManager::initNcurses()
{
    initscr();
    cbreak();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);
    refresh();
}
void WindowManager::releaseNcurses()
{
    for (int i = 0; i < WIN_COUNT; ++i)
    {
        deleteWindow(i);
    }
    endwin();
}

void WindowManager::print(int winId, int x, int y, std::string &str)
{
    print(winId, x, y, str.c_str());
}
void WindowManager::print(int winId, int x, int y, const char *str)
{
    Window *win = getWindow(winId);
    win->printAt(x, y, str);
}

bool WindowManager::read(int winId, int x , int y, const char *str)
{
    Window *win = getWindow(winId);
    return win->readAt(x, y, str);

}
bool WindowManager::read(int winId, int x , int y, std::string &str)
{
    Window *win = getWindow(winId);
    char buff[256];
    bool result = win->readAt(x, y, buff);
    str = buff;
    return result;
}