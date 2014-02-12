#ifndef _WINDOWMANAGER_H_
#define _WINDOWMANAGER_H_

#include <iostream>
#include <cstring>
#include <csignal>
#include <ncurses.h>

#include "display/Window.h"
#include "Grid.h"

#define WIN_COUNT 3
#define WIN_GAME_GRID 0
#define WIN_GAME_TURN 1
#define WIN_SCOREBOARD 2

#define CELL_WIDTH 4
#define CELL_HEIGHT 2

class WindowManager
{
    static bool wich_raised;
    bool initialized;

    Window **win;


    void createWindow(int, int, int, int, int);
    void deleteWindow(int);
    Window *getWindow(int);
    void initWindows();
    void initNcurses();
    void initSignal();
    void releaseNcurses();
    void initColors();


public:
    static void handle_winch(int sig);

    struct WindowSize
    {
        int w;
        int h;
        int x;
        int y;
    };

    WindowManager();
    virtual ~WindowManager();

    void initialize(const char *);

    void leaveCurseMode();
    void restoreCurseMode();

    void setColor(int);
    void clearColor(int);

    int getLines();
    int getCols();

    void print(int, int, int, std::string &);
    void print(int, int, int, const char *);
    bool read(int, int, int, std::string &);
    bool read(int, int, int, const char *);

    void printGrid(int, Grid *, int, int);
    void printBaseGrid(int, Grid *, int, int);

    void getWindowSize(int, WindowSize *);

    void setWindowColor(int, int);
    void clearWindowColor(int, int);

    void refreshAll();

    void update();
};

#endif
