#include "widgets/DisplayGrid.hpp"

#include "widgets/TokenAnimator.hpp"
#include "providers/GravityProvider.hpp"
#include "config/GameSettings.hpp"
#include "Game.hpp"
#include "Grid.hpp"
#include "constants.hpp"

#include "WinnerChecker.hpp"

#include "display/Window.hpp"

#include "Logger.hpp"

DisplayGrid::DisplayGrid(WindowManager *manager, int winId, Game *game) : super(manager, winId)
{
    Logger::log << "Creating DisplayGrid" << std::endl;
    this->game = game;
    this->shiftEnabled = false;

    this->tokenAnimator = new TokenAnimator(manager, winId, game);

}

DisplayGrid::~DisplayGrid()
{
    clearGridArea();
    delete this->tokenAnimator;
}

void DisplayGrid::redrawAll(bool doDrawTokens = true)
{

    Window *win = getWindow();
    if(win == NULL)
        return;

    clearGridArea();
    drawBaseGrid();
    if(doDrawTokens)
        drawTokens();
    drawRuler();

    win->refresh();
}

void DisplayGrid::drawRuler()
{
    Window *win = getWindow();
    int inc = 0;
    char s[10];
    for(int i = 1 + getShiftX() * -1; i <= getWidth(); i++)
    {
        sprintf(s, "%d", static_cast<int>(i));
        win->printAt_unshifted(++inc * CELL_WIDTH, 0, s);
    }
    inc = 0;
    for(int j = 1 + getShiftY() * -1; j <= getHeight(); j++)
    {
        sprintf(s, "%d", static_cast<int>(j));
        win->printAt_unshifted(0, ++inc * CELL_HEIGHT, s);
    }
}

void DisplayGrid::drawBaseGrid()
{
    Window *win = getWindow();
    //win->AttribResetOff();
    chtype drawChar = 0;

    int w_max = getWidth() * CELL_WIDTH ;
    int h_max = getHeight() * CELL_HEIGHT ;

    for(int i = getShiftX() * -1; i <= w_max; i++)
    {
        /*if(i % CELL_WIDTH == 0 && i != 0)
        {
            sprintf(s, "%d", (int)(i / CELL_WIDTH));
            win->printAt(i, 0, s);
        }*/
        for(int j = getShiftY() * -1; j <= h_max; j++)
        {
            /*if(j % CELL_HEIGHT == 0 && j != 0)
            {
                sprintf(s, "%d", (int)(j / CELL_HEIGHT));
                win->printAt(0, j, s);
            }*/
            if(i == 0 && j == 0)
                drawChar = ACS_ULCORNER;
            else if(i == 0 && j == h_max)
                drawChar = ACS_LLCORNER;
            else if(i == w_max && j == 0)
                drawChar = ACS_URCORNER;
            else if(i == w_max && j == h_max)
                drawChar = ACS_LRCORNER;
            else if(i % CELL_WIDTH == 0 && j % CELL_HEIGHT == 0)
                if(i == w_max)
                    drawChar = ACS_RTEE;
                else if(j == h_max)
                    drawChar = ACS_BTEE;
                else if(i == 0)
                    drawChar = ACS_LTEE;
                else if(j == 0)
                    drawChar = ACS_TTEE;
                else
                    drawChar = ACS_PLUS;
            else if(i % CELL_WIDTH == 0)
                drawChar = ACS_VLINE;
            else if(j % CELL_HEIGHT == 0)
                drawChar = ACS_HLINE;
            else
                drawChar = 0;

            if(drawChar != 0)
            {
                win->setChar(i + 2, j + 1, drawChar);
            }
        }
    }
    win->AttribOn(COLOR_PAIR(30));
    switch(this->game->getGrid()->getRotation())
    {
    case 0:
        win->setChar(1, 0, ACS_DIAMOND);
        break;
    case 1:
        win->setChar((1 + getWidth())*CELL_WIDTH - 1, 0, ACS_DIAMOND);
        break;
    case 2:
        win->setChar((1 + getWidth())*CELL_WIDTH - 1, (1 + getHeight())*CELL_HEIGHT, ACS_DIAMOND);
        break;
    case 3:
        win->setChar(1, (1 + getHeight())*CELL_HEIGHT, ACS_DIAMOND);
        break;
    }
    win->AttribOff(COLOR_PAIR(30));
}
void DisplayGrid::clearGridArea()
{
    Window *win = getWindow();
    if(win == NULL)
        return;
    win->clear();
}

void DisplayGrid::drawTokens()
{
    Window *win = getWindow();
    int val;

    char s[4];
    s[0] = ' ';
    s[2] = ' ';
    s[3] = '\0';
    for(int i = getShiftX() * -1; i < getWidth(); i++)
    {
        for(int j = getShiftY() * -1; j < getHeight(); j++)
        {
            val = this->game->getGrid()->getGridAt(i, j);
            if(val > 0)
            {
                s[1] = '0' + static_cast<char>(val);
                win->AttribOn(COLOR_PAIR(val));
            }
            else
            {
                s[1] = ' ';
                win->AttribResetOff();
            }
            win->printAt(i * CELL_WIDTH + 3, j * CELL_HEIGHT + 2, s);
        }
    }
    win->AttribResetOff();
}

void DisplayGrid::init()
{
    Window *win = getWindow();
    if(win == NULL)
        return;


    //this->shiftEnabled = (getWidth() > (win->getWidth() - 2) || getHeight() > (win->getHeight() - 2));

    this->shiftEnabled = true;
    drawBaseGrid();
    drawRuler();
    win->refresh();

}
UpdateState DisplayGrid::update(chtype ch)
{
    if(!this->shiftEnabled)
        return FAILURE;
    switch(ch)
    {
    case 'o':
        setShiftY(getShiftY() - 1);
        break;
    case 'l':
        setShiftY(getShiftY() + 1);
        break;
    case 'k':
        setShiftX(getShiftX() - 1);
        break;
    case 'm':
        setShiftX(getShiftX() + 1);
        break;
    case 'i':
        setShiftX(0);
        setShiftY(0);
    }
    return SUCCESS;
}
void DisplayGrid::render()
{
    Window *win = getWindow();
    if(win == NULL)
        return;

    drawTokens();

    win->refresh();
}

bool DisplayGrid::placeToken(int player, int col)
{
    int y = this->game->getGrid()->getGravityProvider()->getFirstEmptyCell(col);
    if(y > -1)
    {
        this->game->getGrid()->setGridAt(col, y, player);
        this->tokenAnimator->animateToken(player, col, 0, y);
        return true;
    }
    return false;
}

bool DisplayGrid::removeToken(int x, int y)
{
    if(this->game->getGrid()->getGridAt(x, y) > 0)
    {
        this->game->getGrid()->forceSetGridAt(x, y, 0);
        this->game->getGrid()->getGravityProvider()->doColumnGravity(x, std::bind((&TokenAnimator::animateToken), this->tokenAnimator,
                std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4));
        return true;
    }
    //
    return false;
}


void DisplayGrid::rotate(Rotation r)
{
    if(r == ROTATE_CLOCKWISE)
        this->game->getGrid()->rotate(1);
    else if(r == ROTATE_COUNTERCLOCKWISE)
        this->game->getGrid()->rotate(-1);

    redrawAll(game->getGameSettings()->animate);

    this->tokenAnimator->setModifier(2.0);
    this->game->getGrid()->getGravityProvider()->doGravity(std::bind((&TokenAnimator::animateToken), this->tokenAnimator,
            std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4));
    this->tokenAnimator->setModifier(1.0);
}

int DisplayGrid::getShiftX()
{
    return getWindow()->getShiftX() / CELL_WIDTH;
}

int DisplayGrid::getShiftY()
{
    return getWindow()->getShiftY() / CELL_HEIGHT;
}

void DisplayGrid::setShiftX(int x)
{
    Logger::log << (x + getWidth()) * CELL_WIDTH << " <= " << getWindow()->getWidth() - 3 * CELL_WIDTH << std::endl;
    if(x > 0)
        return;
    if((x + getWidth()) * CELL_WIDTH <= getWindow()->getWidth() - 3 * CELL_WIDTH)
        return;
    getWindow()->setShiftX(x * CELL_WIDTH);
    redrawAll();
}

void DisplayGrid::setShiftY(int y)
{
    Logger::log << (y + getHeight()) * CELL_HEIGHT << " <= " << getWindow()->getHeight() - 3 * CELL_HEIGHT << std::endl;
    if(y > 0)
        return;
    if((y + getHeight()) * CELL_HEIGHT <= getWindow()->getHeight() - 3 * CELL_HEIGHT)
        return;
    getWindow()->setShiftY(y * CELL_HEIGHT);
    redrawAll();
}

int DisplayGrid::getWidth()
{
    return game->getGrid()->getWidth();
}
int DisplayGrid::getHeight()
{
    return game->getGrid()->getHeight();
}
