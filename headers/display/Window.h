#ifndef _WINDOW_H_
#define _WINDOW_H_

#include <string>
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
    ~Window();

    void refresh();
    bool printAt(int, int, const char *);
    bool readAt(int, int, const char *);


    int getWidth();
    int getHeight();
    int getX();
    int getY();
    int getId();

};

#endif