#ifndef _WINDOWMANAGER_H_
#define _WINDOWMANAGER_H_

#include <iostream>
#include <cstring>
#include <csignal>
#include <ncurses.h>

class Window;
class Overlay;
#include "constants.h"

class WindowManager
{
    bool initialized;
    bool releasing;

    Window **win;
    Overlay *currentOverlay;


    void createWindow(int, int, int, int, int);
    void deleteWindow(int);
    void initWindows();
    void initNcurses();
    void releaseNcurses();
    void initColors();

    void ntoc(const int &, char *);
    void safe_ntoc(const int &, char *);


public:

    struct WindowSize
    {
        int w;
        int h;
        int x;
        int y;
    };

    WindowManager();
    virtual ~WindowManager();

    Window *getWindow(int);

    void initialize(const char *);

    void leaveCurseMode();
    void restoreCurseMode();

    void setColor(int);
    void clearColor(int);

    int getLines();
    int getCols();

    bool print(int, int, int, std::string &);
    bool printInt(int, int, int, const int);
    bool print(int, int, int, const char *);
    bool print(int, int, int, const char);

    bool append(int, std::string &);
    bool appendInt(int, const int);
    bool append(int, const char *);
    bool append(int, const char);
    bool newLine(int);

    void clearWindow(int);

    bool read(int, int, int, std::string &);
    //void read(int, int, int, const int &);
    bool read(int, int, int, const char *);

    bool getWindowSize(int, WindowSize *);

    void refreshAll();
    void refreshWindow(int);


    void setOverlay(Overlay *);
    void clearOverlay();
    Overlay *getOverlay();
};

#endif
