#include <thread>
#include "widgets/Grid.h"

Grid::Grid(WindowManager *manager, int winId, int width, int height) : Renderable(manager, winId)
{
    this->width = width;
    this->height = height;
    this->totalCells = width * height;
    this->filledCells = 0;
    this->matrice = new int*[width];
    for (int i = 0; i < width; ++i)
    {
        this->matrice[i] = new int[height];
        for (int j = 0; j < height; ++j)
        {
            this->matrice[i][j] = 0;
        }
    }
    currentRotationValue  = 0;
    this->gravityProvider = new GravityProvider(this);
    this->tokenAnimator = new TokenAnimator(manager, winId, this);
}

Grid::~Grid()
{
    clearGridArea();
    for (int i = 0; i < this->width; ++i)
    {
        delete [] this->matrice[i];
    }
    delete [] this->matrice;
    delete this->gravityProvider;
    delete this->tokenAnimator;
}


bool Grid::isDoingSteppedGravity()
{
    return doingSteppedGravity;
}

void Grid::redrawAll()
{

    Window *win = getWindow();
    if(win == NULL)
        return;

    clearGridArea();
    drawBaseGrid();
    drawTokens();

    win->refresh();
}

void Grid::drawBaseGrid()
{
    Window *win = getWindow();
    //win->AttribResetOff();
    chtype drawChar = 0;
    char s[10];

    int h_max = getHeight() * CELL_HEIGHT;
    int w_max = getWidth() * CELL_WIDTH;

    for(int i = 0; i <= w_max; i++)
    {
        if(i % CELL_WIDTH == 0 && i != 0)
        {
            sprintf(s, "%d", (int)(i / CELL_WIDTH));
            win->printAt(i, 0, s);
        }
        for(int j = 0; j <= h_max; j++)
        {
            if(j % CELL_HEIGHT == 0 && j != 0)
            {
                sprintf(s, "%d", (int)(j / CELL_HEIGHT));
                win->printAt(0, j, s);
            }
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
    for(int i = 0; i < getWidth(); i++)
    {
        for(int j = 0; j < getHeight(); j++)
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
    setShiftY(-5);

    doingSteppedGravity = false;
    this->last_x = this->last_y = -1;
    currentRotationValue = 0;
    steppedGravityValue = -1;

    Window *win = getWindow();
    if(win == NULL)
        return;
    drawBaseGrid();
    win->refresh();

}
UpdateState Grid::update(chtype)
{
    /*if(steppedGravityValue != -1 && steppedGravityValue < getHeight())
    {
        ungetch(0);
        updateSteppedGravity();
        std::this_thread::sleep_for (std::chrono::milliseconds(50));
        return OVERRIDE;
    }
    else
    {
        steppedGravityValue = -1;
    }*/
    //doGravity();
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

void Grid::doGravity()
{
    for(int x = 0; x < this->getWidth(); x++)
    {
        for(int y = this->getHeight() - 1; y >= 0; y--)
        {
            y = doGravityForCell(x, y);
        }
    }
}

int Grid::doGravityForCell(int x, int y)
{
    if(getGridAt(x, y) == 0)
    {
        int token_val = getGridAt(x, y - 1);
        if(token_val > 0)
        {
            forceSetGridAt(x, y - 1, 0);
            setGridAt(x, y, token_val);
            y = y + 2;
        }
    }
    return y;
}

void Grid::updateSteppedGravity()
{
    for(int x = 0; x < this->getWidth(); x++)
    {
        for(int y = this->getHeight() - 1; y >= 0; y--)
        {
            doGravityForCell(x, y);
        }
    }
    steppedGravityValue++;
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
        y2 = height - 1 - x;
        break;
    case 2:
        x2 = width - 1 - x;
        y2 = height - 1 - y;
        break;
    case 3:
        x2 = width - 1 - y;
        y2 = x;
        break;
    }
    x = x2;
    y = y2;
}


int Grid::getGridAt(int x, int y)
{
    convertCoords(x, y);
    if((0 <= x && x < width) && (0 <= y && y < height))
        return this->matrice[x][y];
    return -1;
}

bool Grid::setGridAt(int x, int y, int v)
{
    this->last_x = x;
    this->last_y = y;
    convertCoords(x, y);
    if((0 <= x && x < width) && (0 <= y && y < height) && (this->matrice[x][y] == 0))
    {
        this->matrice[x][y] = v;
        return true;
    }
    this->last_x = this->last_y = -1;
    return false;
}

bool Grid::forceSetGridAt(int x, int y, int v)
{
    this->last_x = x;
    this->last_y = y;
    convertCoords(x, y);
    if((0 <= x && x < width) && (0 <= y && y < height))
    {
        this->matrice[x][y] = v;
        return true;
    }
    this->last_x = this->last_y = -1;
    return false;
}

bool Grid::placeToken(int player, int col)
{
    int y = this->gravityProvider->findFirstEmptyCell(col);
    if(y > -1)
    {
        setGridAt(col, y, player);
        this->tokenAnimator->animateToken(player, col, 0, y);
        this->filledCells++;
        return true;
    }
    return false;
}

bool Grid::removeToken(int x, int y)
{
    if(getGridAt(x, y) > 0)
    {
        forceSetGridAt(x, y, 0);
        this->gravityProvider->doColumnGravity(x, std::bind((&TokenAnimator::animateToken), this->tokenAnimator,
                                               std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4));
        //steppedGravityValue = 0;
        //ungetch(0);
        /* doGravity();
         drawTokens();*/
        this->filledCells--;
        return true;
    }
    //
    return false;
}

void Grid::moveToken(int src_x, int src_y, int dst_x, int dst_y)
{
    setGridAt(dst_x, dst_y, getGridAt(src_x, src_y));
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

    Window *win = getWindow();
    if(win == NULL)
        return;
    clearGridArea();
    drawBaseGrid();
    drawTokens();
    win->refresh();

    this->tokenAnimator->setModifier(2.0);
    this->gravityProvider->doGravity(std::bind((&TokenAnimator::animateToken), this->tokenAnimator,
                                     std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4));
    this->tokenAnimator->setModifier(1.0);

    /*steppedGravityValue = 0;
    ungetch(0);*/
    /*doGravity();*/
    //win->refresh();
}


int Grid::getWidth()
{
    if(currentRotationValue == 1 || currentRotationValue == 3)
        return this->height;
    return this->width;
}
int Grid::getHeight()
{
    if(currentRotationValue == 1 || currentRotationValue == 3)
        return this->width;
    return this->height;
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
    getWindow()->setShiftX(x * CELL_WIDTH);
    redrawAll();
}

void Grid::setShiftY(int y)
{
    getWindow()->setShiftY(y * CELL_HEIGHT);
    redrawAll();
}