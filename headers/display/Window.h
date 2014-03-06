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

    int shift_x;
    int shift_y;

    int pos_x;
    int pos_y;

    void applyShift(int &, int &);
public:
    Window(int, int, int, int, int);
    virtual ~Window();

    bool contains(int, int);

    void refresh();
    bool printAt(int, int, const char *);
    bool printAt_unshifted(int, int, const char *);
    bool readAt(int, int, const char *);
    bool readAnyAt(int, int, const char *, const void *);
    bool append(const char *);
    bool newLine();

    void clear();

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

    int getShiftX();
    int getShiftY();

    void setShiftX(int);
    void setShiftY(int);

};

#endif