#include "Grid.h"

#include "Logger.h"
Grid::Grid(int initWidth, int initHeight)
{
    Logger::log << "Creating grid with " << initWidth << "x" << initHeight << std::endl;

    this->initWidth = initWidth;
    this->initHeight = initHeight;
    this->currentRotationValue  = 0;
    this->filledCells = 0;

    this->totalCells = initWidth * initHeight;
    this->matrice = new int*[initWidth];
    for (int i = 0; i < initWidth; ++i)
    {
        this->matrice[i] = new int[initHeight];
        for (int j = 0; j < initHeight; ++j)
        {
            this->matrice[i][j] = 0;
        }
    }
}

Grid::~Grid()
{
    for (int i = 0; i < initWidth; ++i)
    {
        delete [] this->matrice[i];
    }
    delete [] this->matrice;
}

void Grid::logGridDebug(){
    for (int i = 0; i < initWidth; ++i)
    {
        for (int j = 0; j < initHeight; ++j)
        {
            Logger::log << this->matrice[i][j] << " ";
        }
        Logger::log << std::endl;
    }
}


void Grid::convertCoords(int &x, int &y)
{
    int x2(0);
    int y2(0);
    switch(currentRotationValue)
    {
    case 0:
        return;
    case 1:
        x2 = y;
        y2 = initHeight - 1 - x;
        break;
    case 2:
        x2 = initWidth - 1 - x;
        y2 = initHeight - 1 - y;
        break;
    case 3:
        x2 = initWidth - 1 - y;
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
        Logger::log << "getGridAt: [" << x << "," << y << "] is out of bound" << std::endl;
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
        if(v == 0)
            this->filledCells--;
        else
            this->filledCells++;
        this->matrice[x][y] = v;
        return true;
    }
    else
    {
        Logger::log << "setGridAt: [" << x << "," << y << "] cell is not empty" << std::endl;
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
        if(v == 0)
            this->filledCells--;
        else
            this->filledCells++;
        this->matrice[x][y] = v;
        return true;
    }
    else
    {
        Logger::log << "forceSetGridAt: [" << x << "," << y << "] is out of bound" << std::endl;
    }
    this->last_x = this->last_y = -1;
    return false;
}

void Grid::moveGridAt(int src_x, int src_y, int dst_x, int dst_y)
{
    if(setGridAt(dst_x, dst_y, getGridAt(src_x, src_y)))
        forceSetGridAt(src_x, src_y, 0);
}

void Grid::rotate(int rotation)
{
    currentRotationValue += rotation;
    if(currentRotationValue > 3)
        currentRotationValue = 0;
    else if(currentRotationValue < 0)
        currentRotationValue = 3;
}

int Grid::getWidth()
{
    if(currentRotationValue == 1 || currentRotationValue == 3)
        return initHeight;
    return initWidth;
}
int Grid::getHeight()
{
    if(currentRotationValue == 1 || currentRotationValue == 3)
        return initWidth;
    return initHeight;
}

bool Grid::isEmpty()
{
    return this->filledCells == 0;
}
bool Grid::isFull()
{
    return this->filledCells == this->totalCells;
}