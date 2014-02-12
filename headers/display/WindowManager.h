#ifndef _WINDOWMANAGER_H_
#define _WINDOWMANAGER_H_

#include <iostream>
#include <string>
#include <ncurses.h>

#include "display/Window.h"

#define WIN_COUNT 3
#define WIN_GAME_GRID 0
#define WIN_GAME_TURN 1
#define WIN_SCOREBOARD 2

class WindowManager
{
    Window **win;


    void createWindow(int, int, int, int, int);
    void deleteWindow(int);
    Window *getWindow(int);
    void initWindows();
    void initNcurses();
    void releaseNcurses();

public:
    WindowManager();
    ~WindowManager();

    void leaveCurseMode();
    void restoreCurseMode();

    void print(int, int, int, std::string &);
    void print(int, int, int, const char *);
    bool read(int, int, int, std::string &);
    bool read(int, int, int, const char *);

};

#endif
