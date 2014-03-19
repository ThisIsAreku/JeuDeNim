#include "display/WindowManager.h"


#include "Logger.h"
WindowManager::WindowManager()
{
    this->win = new Window*[WIN_COUNT];
    for(int i = 0; i < WIN_COUNT; i++)
        this->win[i] = NULL;

    initialized = false;
    releasing = false;

    initNcurses();

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
    if(winId >= WIN_COUNT || releasing || !initialized)
        return;
    if(this->win[winId] != NULL)
        return;
    this->win[winId] = new Window(winId, w, h, x, y);
}
void WindowManager::deleteWindow(int winId)
{
    if(winId >= WIN_COUNT || !initialized)
        return;
    if(this->win[winId] == NULL)
        return;
    delete this->win[winId];
    this->win[winId] = NULL;
}
Window *WindowManager::getWindow(int winId)
{
    if(winId >= WIN_COUNT || releasing)
        return NULL;
    if(this->win[winId] == NULL)
        initWindows();
    return this->win[winId];
}
void WindowManager::initWindows()
{
    if(releasing || !initialized)
        return;

    double cols = (double)(COLS / 8.);
    int lines = LINES - 6;
    int w_2 = (6 * cols + 1);
    if(w_2 > cols)
        w_2--;

    Logger::log << cols << ", " << lines << " (" << w_2 << ")" << std::endl;

    createWindow(WIN_GAME_GRID,     6 * cols,   lines,  0,              1);
    createWindow(WIN_SCOREBOARD,    2 * cols,       lines,  w_2,   1);
    createWindow(WIN_GAME_TURN,     COLS,   5,      0,              lines + 1);
}
void WindowManager::initNcurses()
{
    if(releasing)
        return;
    if(!initscr())
    {
        Logger::log << "Erreur lors de l'initialisation de NCurses" << std::endl;
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
    releasing = true;
    deleteWindow(WIN_GAME_GRID);
    deleteWindow(WIN_SCOREBOARD);
    deleteWindow(WIN_GAME_TURN);
    endwin();
    initialized = false;
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
    use_default_colors();

    init_pair(1, COLOR_BLACK, COLOR_BLUE);
    init_pair(2, COLOR_BLACK, COLOR_RED);
    init_pair(3, COLOR_BLACK, COLOR_GREEN);
    init_pair(4, COLOR_BLACK, COLOR_YELLOW);

    init_pair(5, COLOR_BLUE, COLOR_WHITE);
    init_pair(6, COLOR_RED, COLOR_WHITE);
    init_pair(7, COLOR_GREEN, COLOR_WHITE);
    init_pair(8, COLOR_YELLOW, COLOR_WHITE);

    init_pair(9, COLOR_BLUE, COLOR_BLACK);
    init_pair(10, COLOR_RED, COLOR_BLACK);
    init_pair(11, COLOR_GREEN, COLOR_BLACK);
    init_pair(12, COLOR_YELLOW, COLOR_BLACK);

    init_pair(30, COLOR_BLACK, COLOR_WHITE);
    use_default_colors();
}

void WindowManager::ntoc(const int &n, char *str)
{
    str[0] = '0' + n;
    str[1] = '\0';
}
void WindowManager::safe_ntoc(const int &, char *)
{
    /*std::ostringstream oss;
    oss << n;
    std::string s = oss.str();
    str[0] = '0' + n;
    str[1] = '\0';*/
}


void WindowManager::initialize(const char *title)
{
    if(initialized)
        return;

    initWindows();
    initColors();

    mvprintw(0, (COLS / 2) - (strlen(title) / 2), title);
    initialized = true;
}


bool WindowManager::print(int winId, int x, int y, std::string &str)
{
    return print(winId, x, y, str.c_str());
}
bool WindowManager::printInt(int winId, int x, int y, const int n)
{
    char str[10];
    sprintf(str, "%d", n);
    return print(winId, x, y, str);
}
bool WindowManager::print(int winId, int x, int y, const char c)
{
    char str[2];
    str[0] = c;
    str[1] = '\0';
    return print(winId, x, y, str);
}
bool WindowManager::print(int winId, int x, int y, const char *str)
{
    if(releasing)
        return false;
    return getWindow(winId)->printAt(x, y, str);
}



bool WindowManager::append(int winId, std::string &str)
{
    return append(winId, str.c_str());
}
bool WindowManager::appendInt(int winId, const int n)
{
    char str[10];
    sprintf(str, "%d", n);
    return append(winId, str);
}
bool WindowManager::append(int winId, const char c)
{
    char str[2];
    str[0] = c;
    str[1] = '\0';
    return append(winId, str);
}
bool WindowManager::append(int winId, const char *str)
{
    if(releasing)
        return false;
    return getWindow(winId)->append(str);
}
bool WindowManager::newLine(int winId)
{
    return getWindow(winId)->newLine();
}




bool WindowManager::read(int winId, int x , int y, const char *str)
{
    if(releasing)
        return false;
    return getWindow(winId)->readAt(x, y, str);

}
bool WindowManager::read(int winId, int x , int y, std::string &str)
{
    char buff[256];
    bool result = read(winId, x, y, buff);
    str = buff;
    return result;
}

bool WindowManager::getWindowSize(int winId, WindowSize *wsize)
{
    if(releasing)
        return false;
    Window *win = getWindow(winId);
    wsize->w = win->getWidth();
    wsize->h = win->getHeight();
    wsize->x = win->getX();
    wsize->y = win->getY();
    return true;
}

void WindowManager::clearWindow(int winId)
{
    if(releasing)
        return;
    getWindow(winId)->clear();
}

int WindowManager::getLines()
{
    return LINES;
}
int WindowManager::getCols()
{
    return COLS;
}

void WindowManager::refreshAll()
{
    if(releasing)
        return;
    for (int i = 0; i < WIN_COUNT; ++i)
    {
        refreshWindow(i);
    }
    refresh();
}

void WindowManager::refreshWindow(int winId)
{
    if(releasing)
        return;
    getWindow(winId)->refresh();
}

void WindowManager::leaveCurseMode()
{
    def_prog_mode();
    endwin();
}
void WindowManager::restoreCurseMode()
{
    reset_prog_mode();
}