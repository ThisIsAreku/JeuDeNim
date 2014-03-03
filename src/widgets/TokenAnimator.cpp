#include <thread>
#include "widgets/TokenAnimator.h"
#include "constants.h"

TokenAnimator::TokenAnimator(WindowManager *manager, int winId, BaseGrid *grid) : Renderable(manager, winId)
{
    _grid = grid;
}

void TokenAnimator::animateToken(int color, int x, int src, int dst)
{
    if(src == dst)
        return;

    Window *win = getWindow();
    if(win == NULL)
        return;

    int timing = 500 / _grid->getHeight();

    int prev = src;
    char token[4];
    token[0] = ' ';
    token[1] = '0' + color;
    token[2] = ' ';
    token[3] = '\0';
    for(int j = src; j <= dst; ++j)
    {
        win->printAt(x * CELL_WIDTH + 3, prev * CELL_HEIGHT + 2, "   ");
        win->AttribOn(COLOR_PAIR(color));
        win->printAt(x * CELL_WIDTH + 3, j * CELL_HEIGHT + 2, token);
        win->AttribOff(COLOR_PAIR(color));
        win->refresh();

        prev = j;
        std::this_thread::sleep_for (std::chrono::milliseconds(timing));

    }
}

void TokenAnimator::init()
{

}
UpdateState TokenAnimator::update(chtype)
{
    return SUCCESS;
}
void TokenAnimator::render()
{

}