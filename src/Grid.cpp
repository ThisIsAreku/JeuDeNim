#include "Grid.h"
#include "providers/DefaultGravityProvider.h"

#include "Logger.h"
Grid::Grid(int initWidth, int initHeight)
{
    Logger::log << "Creating grid with " << initWidth << "x" << initHeight << std::endl;

    this->initWidth = initWidth;
    this->initHeight = initHeight;

    this->currentRotationValue = 0;
    this->filledCells = 0;
    this->modified = false;

    allocate();

    std::fill(this->matriceData, this->matriceData + totalCells, 0);
    std::fill(this->countCellsByPlayers, this->countCellsByPlayers + 4, 0);
}
Grid::Grid(Grid &grid)
{

    this->initWidth = grid.initWidth;
    this->initHeight = grid.initHeight;
    //Logger::log << "Cloning grid with " << initWidth << "x" << initHeight << " (from " << &grid << ")" << std::endl;

    this->currentRotationValue = grid.currentRotationValue;
    this->filledCells = grid.filledCells;
    this->modified = grid.modified;

    allocate();

    std::copy(grid.matriceData, grid.matriceData + totalCells, this->matriceData);
    std::copy(grid.countCellsByPlayers, grid.countCellsByPlayers + 4, this->countCellsByPlayers);
    getGravityProvider()->seekAllFirstEmptyCell();
}

Grid::~Grid()
{
    //Logger::log << "Deleting grid " << this << std::endl;
    delete [] this->matrice;
    delete [] this->matriceData;
    delete gravityProvider;
    delete [] countCellsByPlayers;
}

void Grid::debugGrid() const
{
    Logger::log << std::endl;
    for (int i = 0; i < initWidth; ++i)
    {
        for (int j = 0; j < initHeight; ++j)
        {
            Logger::log << this->matrice[i][j] << " ";
        }
        Logger::log << std::endl;
    }
}

void Grid::clone(Grid &grid)
{
    if(initWidth != grid.initWidth || initHeight != grid.initHeight)
        return;

    this->currentRotationValue = grid.currentRotationValue;
    this->filledCells = grid.filledCells;
    this->modified = grid.modified;

    std::copy(grid.matriceData, grid.matriceData + totalCells, this->matriceData);
    std::copy(grid.countCellsByPlayers, grid.countCellsByPlayers + 4, this->countCellsByPlayers);
    getGravityProvider()->seekAllFirstEmptyCell();

}
Grid &Grid::operator =(Grid b)
{
    clone(b);
    return (*this);
}

GravityProvider *Grid::getGravityProvider()
{
    return this->gravityProvider;
}

void Grid::allocate()
{
    this->totalCells = initWidth * initHeight;
    this->matrice = new int*[initWidth];
    this->matriceData = new int[totalCells];

    for (int i = 0; i < initWidth; ++i)
    {
        this->matrice[i] = this->matriceData + (i * initHeight);
    }

    this->gravityProvider = new DefaultGravityProvider(this);
    this->countCellsByPlayers = new int[4];
}

void Grid::convertCoords(int &x, int &y) const
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

int Grid::getGridAt(int x, int y) const
{
    if((0 <= x && x < getWidth()) && (0 <= y && y < getHeight()))
    {
        convertCoords(x, y);
        //Logger::log << "getGridAt: [" << x << "," << y << "] is " << this->matrice[x][y] << std::endl;
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
    if(v == 0)
    {
        Logger::log << "setGridAt: [" << x << "," << y << "] v == 0" << std::endl;
        return false;
    }
    if(getGridAt(x, y) == 0)
    {
        this->filledCells++;
        this->countCellsByPlayers[v - 1]++;
        //Logger::log << "setGridAt: [" << x << "," << y << "] ";

        getGravityProvider()->changeColumnFirstEmptyCell(x, -1);
        convertCoords(x, y);
        this->matrice[x][y] = v;
        modified = true;
        return true;
    }
    else
    {
        Logger::log << "setGridAt: [" << x << "," << y << "] cell is not empty" << std::endl;
    }
    return false;
}

bool Grid::forceSetGridAt(int x, int y, int v)
{
    if((0 <= x && x < getWidth()) && (0 <= y && y < getHeight()))
    {
        int prev = getGridAt(x, y);
        if(v == 0)
        {
            if(prev > 0)
                this->countCellsByPlayers[prev - 1]--;
            this->filledCells--;
            getGravityProvider()->changeColumnFirstEmptyCell(x, 1);
        }
        else
        {
            if(prev > 0)
                this->countCellsByPlayers[prev - 1]--;
            this->countCellsByPlayers[v - 1]++;
            this->filledCells++;
            getGravityProvider()->changeColumnFirstEmptyCell(x, -1);
        }

        convertCoords(x, y);
        this->matrice[x][y] = v;
        modified = true;
        return true;
    }
    else
    {
        Logger::log << "forceSetGridAt: [" << x << "," << y << "] is out of bound" << std::endl;
    }
    return false;
}

void Grid::moveGridAt(int src_x, int src_y, int dst_x, int dst_y)
{
    if(src_x != dst_x)
        return;
    if((0 <= src_x && src_x < getWidth()) && (0 <= src_y && src_y < getHeight()) && (0 <= dst_y && dst_y < getHeight()))
    {
        convertCoords(src_x, src_y);
        convertCoords(dst_x, dst_y);
        this->matrice[dst_x][dst_y] = this->matrice[src_x][src_y];
        this->matrice[src_x][src_y] = 0;
        modified = true;
    }
}

void Grid::rotate(int rotation)
{
    currentRotationValue += rotation;
    if(currentRotationValue > 3)
        currentRotationValue = 0;
    else if(currentRotationValue < 0)
        currentRotationValue = 3;

    modified = true;
}

bool Grid::isModified() const
{
    return this->modified;
}
void Grid::resetModified()
{
    this->modified = false;
}

int Grid::getWidth() const
{
    if(currentRotationValue == 1 || currentRotationValue == 3)
        return initHeight;
    return initWidth;
}
int Grid::getHeight() const
{
    if(currentRotationValue == 1 || currentRotationValue == 3)
        return initWidth;
    return initHeight;
}

bool Grid::isEmpty() const
{
    return this->filledCells == 0;
}
bool Grid::isFull() const
{
    return this->filledCells == this->totalCells;
}
int Grid::getFilledCells() const
{
    return this->filledCells;
}
int Grid::getTotalCells() const
{
    return this->totalCells;
}
int Grid::getCellsForPlayer(const int &p) const
{
    return this->countCellsByPlayers[p];
}
int Grid::getRotation() const
{
    return this->currentRotationValue;
}

std::ostream &operator <<(std::ostream &a, Grid &b)
{
    a << b.initWidth << " " << b.initHeight << std::endl << b.currentRotationValue << " " << b.filledCells << std::endl;
    for (int i = 0; i < b.totalCells; ++i)
    {
        a << b.matriceData[i] << " ";
    }
    a << std::endl;
    for (int i = 0; i < 4; ++i)
    {
        a << b.countCellsByPlayers[i] << " ";
    }
    a << std::endl;
    return a;
}

std::istream &operator >>(std::istream &a, Grid &b)
{
    delete [] b.matrice;
    delete [] b.matriceData;

    a >> b.initWidth >> b.initHeight >> b.currentRotationValue >> b.filledCells;

    b.totalCells = b.initWidth * b.initHeight;
    b.matrice = new int*[b.initWidth];
    b.matriceData = new int[b.totalCells];

    for (int i = 0; i < b.initWidth; ++i)
    {
        b.matrice[i] = b.matriceData + (i * b.initHeight);
    }

    for (int i = 0; i < b.totalCells; ++i)
    {
        a >> b.matriceData[i];
    }

    for (int i = 0; i < 4; ++i)
    {
        a >> b.countCellsByPlayers[i];
    }


    b.getGravityProvider()->seekAllFirstEmptyCell();

    return a;
}