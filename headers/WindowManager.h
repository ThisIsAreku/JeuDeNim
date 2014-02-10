#ifndef _WINDOWMANAGER_H_
#define _WINDOWMANAGER_H_

#include <ncurses.h>

#define WIN_COUNT 2
#define WIN_GAME_GRID 0
#define WIN_SCOREBOARD 1

class WindowManager
{
    WINDOW **win;


    void createWindow(int, int, int, int, int);
    void deleteWindow(int);
    WINDOW *getWindow(int);
    void initWindows();
    void initNcurses();
    void releaseNcurses();

public:
    WindowManager();
    ~WindowManager();

    void print(int, int, int, std::string);

};

#endif
