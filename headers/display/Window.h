#ifndef _WINDOW_H_
#define _WINDOW_H_

#include <cstring>
#include <iostream>
#include <ncurses.h>

class Window
{
    WINDOW *handle;
    int id;
    int width;
    int height;
    int x;
    int y;
public:
    Window(int, int, int, int, int);
    virtual ~Window();

    void refresh();
    bool printAt(int, int, const char *);
    bool readAt(int, int, const char *);

    void AttribOn(int);
    void AttribOff(int);
    void AttribSet(int);
    void AttribResetOff();
    void AttribResetOn();

    void setChar(int, int, chtype);


    int getWidth();
    int getHeight();
    int getX();
    int getY();
    int getId();

};

#endif