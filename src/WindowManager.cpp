#include <iostream>
#include "WindowManager.h"

WindowManager::WindowManager(){
    this->win = new WINDOW*[WIN_COUNT];
    //this->grille = new Grille(10, 10);
    initNcurses();
    initWindows();

}
WindowManager::~WindowManager(){
    releaseNcurses();
    delete this->win;
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
    WINDOW* newWindow = newwin(w, h, x, y);
    box(newWindow, 0 , 0);
    wrefresh(newWindow);
    this->win[winId] = newWindow;
}
void WindowManager::deleteWindow(int winId)
{
    if(winId >= WIN_COUNT)
        return;
    if(this->win[winId] == NULL)
        return;
    WINDOW* newWindow = this->win[winId];
    wborder(newWindow, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
    wrefresh(newWindow);
    delwin(newWindow);
    this->win[winId] = NULL;
}
WINDOW *WindowManager::getWindow(int winId)
{
    if(winId >= WIN_COUNT)
        return NULL;
    if(this->win[winId] == NULL)
        initWindows();
    return this->win[winId];
}
void WindowManager::initWindows(){
    int cols = COLS / 3;

    createWindow(WIN_GAME_GRID, LINES-1, 2*cols, 0, 0);
    createWindow(WIN_SCOREBOARD, LINES-1, cols, 0, 2*cols+1);
}
void WindowManager::initNcurses()
{
    initscr();
    cbreak();
    noecho();
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


void WindowManager::print(int winId, int x, int y, std::string str)
{    
    WINDOW* win = getWindow(WIN_SCOREBOARD);
    mvwprintw(win, x, y, str.c_str());
    wrefresh(win);
    int ch;
    while((ch = getch()) != KEY_F(2))
    {
    }
}