#ifndef _CELL_CURSOR_H_
#define _CELL_CURSOR_H_

#include <functional>   // std::bind

#include "display/Renderable.hpp"

class Grid;

class CellCursor : public Renderable
{
    typedef Renderable super;
    std::function<bool(int, int)> cellSelected_callback;
    std::function<void( )> selectionCanceled_callback;
    int color;
    int x;
    int y;
    int prev_x;
    int prev_y;

    int max_x;
    int max_y;

    int cursor_height;
    int cursor_width;

    bool visible;

    Grid *grid;

    void drawBox(int x, int y);
    void drawBox();

    void constraintCursor();
public:
    CellCursor(WindowManager *, int, Grid *, int);
    virtual ~CellCursor();

    void setCellSelectedListener(std::function<bool(int, int)>);
    void setSelectionCanceledListener(std::function<void()>);

    void setCursorSize(int, int);
    void setCursorPosition(int, int);
    void setVisible(bool);
    bool isVisible();

    void init();
    UpdateState update(chtype);
    void render();
};

#endif
