#include <sstream>
#include "display/WindowManager.h"

bool WindowManager::wich_raised = false;

WindowManager::WindowManager()
{
    this->win = new Window*[WIN_COUNT];
    for(int i = 0; i < WIN_COUNT; i++)
        this->win[i] = NULL;
    //this->grille = new Grille(10, 10);
    initNcurses();
    initialized = false;

}
WindowManager::~WindowManager()
{
    releaseNcurses();
    delete [] this->win;
}

/************/
/* PRIVATES */
/************/
void WindowManager::createWindow(int winId, int w, int h, int x, int y)
{
    if(winId >= WIN_COUNT)
        return;
    if(this->win[winId] != NULL)
        return;
    this->win[winId] = new Window(winId, w, h, x, y);
}
void WindowManager::deleteWindow(int winId)
{
    if(winId >= WIN_COUNT)
        return;
    if(this->win[winId] == NULL)
        return;
    delete this->win[winId];
    this->win[winId] = NULL;
}
Window *WindowManager::getWindow(int winId)
{
    if(winId >= WIN_COUNT)
        return NULL;
    if(this->win[winId] == NULL)
        initWindows();
    return this->win[winId];
}
void WindowManager::initWindows()
{

    int cols = COLS / 3;
    int lines = LINES - 6;

    createWindow(WIN_GAME_GRID,     2 * cols,   lines,  0,              1);
    createWindow(WIN_SCOREBOARD,    cols,       lines,  2 * cols + 1,   1);
    createWindow(WIN_GAME_TURN,     COLS - 1,   5,      0,              lines + 1);
}
void WindowManager::initNcurses()
{
    if(!initscr())
    {
        std::cerr << "Erreur lors de l'initialisation de NCurses" << std::endl;
        exit(1);
    }
    cbreak();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);
    refresh();
}
void WindowManager::releaseNcurses()
{
    for (int i = 0; i < WIN_COUNT; ++i)
    {
        deleteWindow(i);
    }
    endwin();
}
void WindowManager::initColors()
{
    if(!has_colors())
    {
        const std::string no_colors("Pas de couleurs :(");
        mvprintw(0, COLS - no_colors.length(), no_colors.c_str());
    }
    refresh();
    start_color();
    init_color(COLOR_RED, 700, 0, 0);
    init_color(COLOR_BLUE, 0, 0, 700);
    init_pair(1, COLOR_BLACK, COLOR_RED);
    init_pair(2, COLOR_BLACK, COLOR_BLUE);
    init_pair(3, COLOR_BLACK, COLOR_GREEN);
    init_pair(4, COLOR_BLACK, COLOR_YELLOW);
    use_default_colors();
}
void WindowManager::initSignal()
{
    signal((int)SIGWINCH, WindowManager::handle_winch);
}
void WindowManager::handle_winch(int sig)
{
    wich_raised = true;
}


void WindowManager::initialize(const char *title)
{
    if(initialized)
        return;
    initialized = true;

    initWindows();
    initColors();

    mvprintw(0, (COLS / 2) - (strlen(title) / 2), title);
}


void WindowManager::print(int winId, int x, int y, std::string &str)
{
    print(winId, x, y, str.c_str());
}
void WindowManager::print(int winId, int x, int y, const char *str)
{
    getWindow(winId)->printAt(x, y, str);
}

bool WindowManager::read(int winId, int x , int y, const char *str)
{
    Window *win = getWindow(winId);
    return win->readAt(x, y, str);

}
bool WindowManager::read(int winId, int x , int y, std::string &str)
{
    Window *win = getWindow(winId);
    char buff[256];
    bool result = win->readAt(x, y, buff);
    str = buff;
    return result;
}

void WindowManager::getWindowSize(int winId, WindowSize *wsize)
{
    Window *win = getWindow(winId);
    wsize->w = win->getWidth();
    wsize->h = win->getHeight();
    wsize->x = win->getX();
    wsize->y = win->getY();
}

int WindowManager::getLines()
{
    return LINES;
}
int WindowManager::getCols()
{
    return COLS;
}
void WindowManager::printBaseGrid(int winId, Grid *grid, int x, int y)
{
    Window *win = getWindow(winId);
    //win->AttribResetOff();
    chtype drawChar = 0;

    int h_max = grid->getHeight() * CELL_HEIGHT;
    int w_max = grid->getWidth() * CELL_WIDTH;

    for(int i = 0; i <= h_max; i++)
    {
        for(int j = 0; j <= w_max; j++)
        {
            if(i == 0 && j == 0)
                drawChar = ACS_ULCORNER;
            else if(i == 0 && j == w_max)
                drawChar = ACS_URCORNER;
            else if(i == h_max && j == 0)
                drawChar = ACS_LLCORNER;
            else if(i == h_max && j == w_max)
                drawChar = ACS_LRCORNER;
            else if(i % CELL_HEIGHT == 0 && j % CELL_WIDTH == 0)
                if(i == h_max)
                    drawChar = ACS_BTEE;
                else if(j == w_max)
                    drawChar = ACS_RTEE;
                else if(i == 0)
                    drawChar = ACS_TTEE;
                else if(j == 0)
                    drawChar = ACS_LTEE;
                else
                    drawChar = ACS_PLUS;
            else if(i % CELL_HEIGHT == 0)
                drawChar = ACS_HLINE;
            else if(j % CELL_WIDTH == 0)
                drawChar = ACS_VLINE;
            else
                drawChar = 0;

            if(drawChar != 0)
                win->setChar(i + x, j + y, drawChar);
        }
    }
    win->refresh();
}

void WindowManager::printGrid(int winId, Grid *grid, int x, int y)
{
    Window *win = getWindow(winId);
    int val;
    std::ostringstream oss;
    std::string s;
    for(int i = 0; i < grid->getWidth(); i++)
    {
        for(int j = 0; j < grid->getHeight(); j++)
        {
            val = grid->getGridAt(i, j);
            if(val > 0)
            {
                oss << " " << val << " ";
                win->AttribOn(COLOR_PAIR(val));
                win->printAt(i * CELL_WIDTH + x, j * CELL_HEIGHT + y, oss.str().c_str());
                win->AttribOff(COLOR_PAIR(val));
                oss.str("");
                oss.clear();
            }
        }
    }
}

void WindowManager::refreshAll()
{
    endwin();
    refresh();
    //initscr();
    for (int i = 0; i < WIN_COUNT; ++i)
    {
        getWindow(i)->refresh();
    }
}

void WindowManager::update()
{
    if(WindowManager::wich_raised){
        WindowManager::wich_raised = false;
        refreshAll();
    }
}