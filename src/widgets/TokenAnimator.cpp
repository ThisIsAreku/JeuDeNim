#include <thread>
#include "widgets/TokenAnimator.h"
#include "constants.h"

TokenAnimator::TokenAnimator(WindowManager *manager, int winId, BaseGrid *grid) : Renderable(manager, winId)
{
    _grid = grid;
    modifier = 1.0;
}
void TokenAnimator::setModifier(double d)
{
    modifier = d;
}

void TokenAnimator::animateToken(int color, int x, int src, int dst)
{
    if(src == dst)
        return;

    Window *win = getWindow();
    if(win == NULL)
        return;

    int timing = 500 / (_grid->getHeight() * modifier);

    if(timing == 0)
        timing = 1;
    timeout(timing); // content !

    int prev = src;
    char token[4];
    token[0] = ' ';
    token[1] = '0' + color;
    token[2] = ' ';
    token[3] = '\0';
    for(int j = src; j <= dst; ++j)
    {
        if((int)getch() != -1)
            break;
        win->printAt(x * CELL_WIDTH + 3, prev * CELL_HEIGHT + 2, "   ");
        win->AttribOn(COLOR_PAIR(color));
        win->printAt(x * CELL_WIDTH + 3, j * CELL_HEIGHT + 2, token);
        win->AttribOff(COLOR_PAIR(color));
        win->refresh();
        prev = j;

        //probleme: les touches en attentent restent en attente !
        //std::this_thread::sleep_for (std::chrono::milliseconds(timing));

    }
    win->printAt(x * CELL_WIDTH + 3, prev * CELL_HEIGHT + 2, "   ");
    win->AttribOn(COLOR_PAIR(color));
    win->printAt(x * CELL_WIDTH + 3, dst * CELL_HEIGHT + 2, token);
    win->AttribOff(COLOR_PAIR(color));
    win->refresh();

    timeout(-1); // reset buffer bloquant
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