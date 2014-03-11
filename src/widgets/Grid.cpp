#include <thread>
#include "widgets/Grid.h"
#include "providers/GravityProvider.h"

#include "WinnerChecker.h"

Grid::Grid(WindowManager *manager, int winId, BaseGame *baseGame) : Renderable(manager, winId)
{
    this->baseGame = baseGame;
    this->currentRotationValue  = 0;
    this->filledCells = 0;
    this->shiftEnabled = false;

    this->totalCells = getWidth() * getHeight();
    this->matrice = new int*[getWidth()];
    for (int i = 0; i < getWidth(); ++i)
    {
        this->matrice[i] = new int[getHeight()];
        for (int j = 0; j < getHeight(); ++j)
        {
            this->matrice[i][j] = 0;
        }
    }

    this->tokenAnimator = new TokenAnimator(manager, winId, this);

}

Grid::~Grid()
{
    clearGridArea();
    for (int i = 0; i < getWidth(); ++i)
    {
        delete [] this->matrice[i];
    }
    delete [] this->matrice;
    delete this->tokenAnimator;
}

void Grid::redrawAll()
{

    Window *win = getWindow();
    if(win == NULL)
        return;

    clearGridArea();
    drawBaseGrid();
    drawTokens();
    drawRuler();

    win->refresh();
}

void Grid::drawRuler()
{
    Window *win = getWindow();
    int inc = 0;
    char s[10];
    for(int i = 1 + getShiftX() * -1; i <= getWidth(); i++)
    {
        sprintf(s, "%d", (int)(i));
        win->printAt_unshifted(++inc * CELL_WIDTH, 0, s);
    }
    inc = 0;
    for(int j = 1 + getShiftY() * -1; j <= getHeight(); j++)
    {
        sprintf(s, "%d", (int)(j));
        win->printAt_unshifted(0, ++inc * CELL_HEIGHT, s);
    }
}

void Grid::drawBaseGrid()
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
    switch(currentRotationValue)
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
void Grid::clearGridArea()
{
    Window *win = getWindow();
    if(win == NULL)
        return;
    win->clear();
}

void Grid::drawTokens()
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
            val = getGridAt(i, j);
            if(val > 0)
            {
                s[1] = '0' + val;
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

void Grid::init()
{
    this->last_x = this->last_y = -1;
    currentRotationValue = 0;

    Window *win = getWindow();
    if(win == NULL)
        return;


    //this->shiftEnabled = (getWidth() > (win->getWidth() - 2) || getHeight() > (win->getHeight() - 2));

    this->shiftEnabled = true;
    drawBaseGrid();
    drawRuler();
    win->refresh();

}
UpdateState Grid::update(chtype ch)
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
void Grid::render()
{
    Window *win = getWindow();
    if(win == NULL)
        return;

    drawTokens();
    /*char r[3] = {'r', ':', '0'};
    r[2] += currentRotationValue;
    win->printAt(win->getWidth() - 10, 0, r);*/
    win->refresh();
}

void Grid::convertCoords(int &x, int &y)
{
    int x2;
    int y2;
    switch(currentRotationValue)
    {
    case 0:
        return;
    case 1:
        x2 = y;
        y2 = this->baseGame->getGameSettings()->getBoardHeight() - 1 - x;
        break;
    case 2:
        x2 = this->baseGame->getGameSettings()->getBoardWidth() - 1 - x;
        y2 = this->baseGame->getGameSettings()->getBoardHeight() - 1 - y;
        break;
    case 3:
        x2 = this->baseGame->getGameSettings()->getBoardWidth() - 1 - y;
        y2 = x;
        break;
    }
    x = x2;
    y = y2;
}


int Grid::getGridAt(int x, int y)
{
    if((0 <= x && x < getWidth()) && (0 <= y && y < getHeight()))
    {
        convertCoords(x, y);
        return this->matrice[x][y];
    }
    else
    {
        std::cerr << "getGridAt: [" << x << "," << y << "] is out of bound" << std::endl;
    }
    return -1;
}

bool Grid::setGridAt(int x, int y, int v)
{
    this->last_x = x;
    this->last_y = y;
    if(getGridAt(x, y) == 0)
    {
        convertCoords(x, y);
        this->matrice[x][y] = v;
        return true;
    }
    else
    {
        std::cerr << "setGridAt: [" << x << "," << y << "] cell is not empty" << std::endl;
    }
    this->last_x = this->last_y = -1;
    return false;
}

bool Grid::forceSetGridAt(int x, int y, int v)
{
    this->last_x = x;
    this->last_y = y;
    if((0 <= x && x < getWidth()) && (0 <= y && y < getHeight()))
    {
        convertCoords(x, y);
        this->matrice[x][y] = v;
        return true;
    }
    else
    {
        std::cerr << "forceSetGridAt: [" << x << "," << y << "] is out of bound" << std::endl;
    }
    this->last_x = this->last_y = -1;
    return false;
}

bool Grid::placeToken(int player, int col)
{
    int y = this->baseGame->getGravityProvider()->findFirstEmptyCell(col);
    if(y > -1)
    {
        this->filledCells++;
        setGridAt(col, y, player);
        this->tokenAnimator->animateToken(player, col, 0, y);
        this->baseGame->getWinnerChecker()->onPlaceToken(col, y);
        return true;
    }
    return false;
}

bool Grid::removeToken(int x, int y)
{
    if(getGridAt(x, y) > 0)
    {
        this->filledCells--;
        forceSetGridAt(x, y, 0);
        this->baseGame->getGravityProvider()->doColumnGravity(x, std::bind((&TokenAnimator::animateToken), this->tokenAnimator,
                std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4));

        this->baseGame->getWinnerChecker()->onRemoveToken(x, y);
        return true;
    }
    //
    return false;
}

void Grid::moveToken(int src_x, int src_y, int dst_x, int dst_y)
{
    if(setGridAt(dst_x, dst_y, getGridAt(src_x, src_y)))
        forceSetGridAt(src_x, src_y, 0);
}


void Grid::rotate(EntityTurnAction r)
{
    if(r == ROTATE_CLOCKWISE)
    {
        currentRotationValue++;
    }
    else if(r == ROTATE_COUNTERCLOCKWISE)
    {
        currentRotationValue--;
    }
    if(currentRotationValue > 3)
        currentRotationValue = 0;
    else if(currentRotationValue < 0)
        currentRotationValue = 3;

    redrawAll();

    this->tokenAnimator->setModifier(2.0);
    this->baseGame->getGravityProvider()->doGravity(std::bind((&TokenAnimator::animateToken), this->tokenAnimator,
            std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4));
    this->tokenAnimator->setModifier(1.0);


    this->baseGame->getWinnerChecker()->onRotate();
}


int Grid::getWidth()
{
    if(currentRotationValue == 1 || currentRotationValue == 3)
        return this->baseGame->getGameSettings()->getBoardHeight();
    return this->baseGame->getGameSettings()->getBoardWidth();
}
int Grid::getHeight()
{
    if(currentRotationValue == 1 || currentRotationValue == 3)
        return this->baseGame->getGameSettings()->getBoardWidth();
    return this->baseGame->getGameSettings()->getBoardHeight();
}

bool Grid::isEmpty()
{
    return this->filledCells == 0;
}
bool Grid::isFull()
{
    return this->filledCells == this->totalCells;
}

int Grid::getShiftX()
{
    return getWindow()->getShiftX() / CELL_WIDTH;
}

int Grid::getShiftY()
{
    return getWindow()->getShiftY() / CELL_HEIGHT;
}

void Grid::setShiftX(int x)
{
    std::cerr << (x + getWidth()) * CELL_WIDTH << " <= " << getWindow()->getWidth() - 3 * CELL_WIDTH << std::endl;
    if(x > 0)
        return;
    if((x + getWidth()) * CELL_WIDTH <= getWindow()->getWidth() - 3 * CELL_WIDTH)
        return;
    getWindow()->setShiftX(x * CELL_WIDTH);
    redrawAll();
}

void Grid::setShiftY(int y)
{
    std::cerr << (y + getHeight()) * CELL_HEIGHT << " <= " << getWindow()->getHeight() - 3 * CELL_HEIGHT << std::endl;
    if(y > 0)
        return;
    if((y + getHeight()) * CELL_HEIGHT <= getWindow()->getHeight() - 3 * CELL_HEIGHT)
        return;
    getWindow()->setShiftY(y * CELL_HEIGHT);
    redrawAll();
}