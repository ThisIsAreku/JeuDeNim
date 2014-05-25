#include "widgets/TokenAnimator.h"

#include "config/GameSettings.h"
#include "Game.h"
#include "Grid.h"

#include "constants.h"
#include "display/Window.h"

#include "Logger.h"

TokenAnimator::TokenAnimator(WindowManager *manager, int winId, Game *game) : super(manager, winId)
{
    this->game = game;
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

    char token[4];
    token[0] = ' ';
    token[1] = '0' + static_cast<char>(color);
    token[2] = ' ';
    token[3] = '\0';

    if(!game->getGameSettings()->animate)  // affiche directement a dst
    {
        win->AttribOn(COLOR_PAIR(color));
        win->printAt(x * CELL_WIDTH + 3, dst * CELL_HEIGHT + 2, token);
        win->AttribOff(COLOR_PAIR(color));
        win->refresh();
        return;
    }

    int timing = static_cast<int>(400 / (game->getGrid()->getHeight() * modifier));

    if(timing == 0)
        timing = 1;
    timeout(timing); // content !

    int prev = src;
    for(int j = src; j <= dst; ++j)
    {
        chtype t = static_cast<chtype>(getch());
        game->doKeyboardActions(t);
        if(static_cast<int>(t) != -1)
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
