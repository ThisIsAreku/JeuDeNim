#include "widgets/TokenLiner.hpp"

#include "display/Window.hpp"
#include "constants.hpp"

#include "structs/TokenAlignement.hpp"

#include "Logger.hpp"

TokenLiner::TokenLiner(WindowManager *manager, int winId) : super(manager, winId) {}

void TokenLiner::displayAlignement(TokenAlignement align)
{
    Window *win = getWindow();
    if(win == NULL)
        return;

    int inc_x(0), inc_y(0);
    if(align.orientation == HORIZONTAL)
        inc_x = 1;
    else if(align.orientation == VERTICAL)
        inc_y = 1;
    else if(align.orientation == DIAGONAL)
    {
        if(align.x1 < align.x2)
            inc_x = 1;
        else if(align.x1 > align.x2)
            inc_x = -1;

        if(align.y1 < align.y2)
            inc_y = 1;
        else if(align.y1 > align.y2)
            inc_y = -1;
    }

    int t_x = align.x1;
    int t_y = align.y1;
    char token[4];
    token[0] = ' ';
    token[1] = '0' + static_cast<char>(align.playerId + 1);
    token[2] = ' ';
    token[3] = '\0';

    Logger::log << "Drawing winner alignement" << std::endl;
    Logger::log << "X: " << align.x1 << " -> " << align.x2 << " (" << inc_x << ")" << std::endl;
    Logger::log << "Y: " << align.y1 << " -> " << align.y2 << " (" << inc_y << ")" << std::endl;

    while(t_x != align.x2 || t_y != align.y2)
    {
        win->AttribOn(COLOR_PAIR(align.playerId + 5));
        win->printAt(t_x * CELL_WIDTH + 3, t_y * CELL_HEIGHT + 2, token);
        win->AttribOff(COLOR_PAIR(align.playerId + 5));
        Logger::log << t_x << "," << t_y << std::endl;


        t_x += inc_x;
        t_y += inc_y;
    }
    win->AttribOn(COLOR_PAIR(align.playerId + 5));
    win->printAt(t_x * CELL_WIDTH + 3, t_y * CELL_HEIGHT + 2, token);
    win->AttribOff(COLOR_PAIR(align.playerId + 5));
    Logger::log << "Last: " << t_x << "," << t_y << std::endl;
    win->refresh();
}

void TokenLiner::init()
{

}
UpdateState TokenLiner::update(chtype)
{
    return SUCCESS;
}
void TokenLiner::render()
{

}
