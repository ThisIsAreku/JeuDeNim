#include <sstream>

#include "widgets/CellCursor.hpp"

#include "Grid.hpp"
#include "constants.hpp"

#include "display/Window.hpp"

#include "Logger.hpp"
CellCursor::CellCursor(WindowManager *manager, int winId, Grid *grid, int color) : super(manager, winId)
{
    this->color = color;

    this->cellSelected_callback = NULL;
    this->selectionCanceled_callback = NULL;

    this->cursor_width = this->cursor_height = 1;

    this->grid = grid;

    this->visible = true;

    this->x = this->prev_x = 0;
    this->y = this->prev_y = 0;

    this->max_x = this->grid->getWidth() - this->cursor_width;
    this->max_y = this->grid->getHeight() - this->cursor_height;
}
CellCursor::~CellCursor()
{
    Window *win = getWindow();
    if(win == NULL)
        return;
    win->AttribResetOff();
    drawBox(x, y);
    win->refresh();
}


void CellCursor::setCellSelectedListener(std::function<bool(int, int)> c)
{
    this->cellSelected_callback = c;
}
void CellCursor::setSelectionCanceledListener(std::function<void()> c)
{
    this->selectionCanceled_callback = c;
}
void CellCursor::setCursorPosition(int x, int y)
{
    this->prev_x = this->x = x;
    this->prev_y = this->y = y;

    constraintCursor();
}

void CellCursor::setCursorSize(int w, int h)
{
    // reset cursor before showing
    if(visible)
    {
        Logger::log << "visible" << std::endl;
        Window *win = getWindow();
        if(win == NULL)
            return;

        win->AttribResetOff();
        drawBox(x, y);
        win->refresh();
    }

    cursor_width = w;
    cursor_height = h;

    constraintCursor();
}

void CellCursor::setVisible(bool visible)
{
    this->visible = visible;
    constraintCursor();
    if(visible)
        drawBox();
    else
    {
        Window *win = getWindow();
        if(win == NULL)
            return;

        win->AttribResetOff();
        drawBox(x, y);
        win->refresh();
    }
}
bool CellCursor::isVisible()
{
    return this->visible;
}

void CellCursor::drawBox(int pos_x, int pos_y)
{
    Window *win = getWindow();
    if(win == NULL)
        return;

    int full_x_coords = this->grid->getWidth() * CELL_WIDTH;
    int full_y_coords = this->grid->getHeight() * CELL_HEIGHT;

    int cursor_coords_width = this->cursor_width * CELL_WIDTH;
    int cursor_coords_height = this->cursor_height * CELL_HEIGHT;

    int x_coords = pos_x * CELL_WIDTH;
    int y_coords = pos_y * CELL_HEIGHT;

    int max_x_coords = x_coords + cursor_coords_width;
    int max_y_coords = y_coords + cursor_coords_height;

    chtype drawChar = ACS_PLUS;
    for (int i = x_coords; i <= max_x_coords; ++i)
    {
        for (int j = y_coords; j <= max_y_coords; ++j)
        {
            if(i % cursor_coords_width != 0 && j % cursor_height != 0)
                continue;

            if(i == 0 && j == 0)
                drawChar = ACS_ULCORNER;
            else if(i == 0 && j == full_y_coords)
                drawChar = ACS_LLCORNER;
            else if(i == full_x_coords && j == 0)
                drawChar = ACS_URCORNER;
            else if(i == full_x_coords && j == full_y_coords)
                drawChar = ACS_LRCORNER;
            else if(i % CELL_WIDTH == 0 && j % CELL_HEIGHT == 0)
                if(i == full_x_coords)
                    drawChar = ACS_RTEE;
                else if(j == full_y_coords)
                    drawChar = ACS_BTEE;
                else if(i == 0)
                    drawChar = ACS_LTEE;
                else if(j == 0)
                    drawChar = ACS_TTEE;
                else
                    drawChar = ACS_PLUS;
            else if(i % cursor_coords_width == 0)
                drawChar = ACS_VLINE;
            else if(j % cursor_coords_height == 0)
                drawChar = ACS_HLINE;
            else
                drawChar = 0;

            if(drawChar != 0)
                win->setChar(i + 2, j + 1, drawChar);
        }
    }
}

void CellCursor::drawBox()
{
    Window *win = getWindow();
    if(win == NULL)
        return;

    win->AttribResetOff();
    drawBox(prev_x, prev_y);

    win->AttribOn(COLOR_PAIR(this->color));
    drawBox(x, y);
    win->AttribOff(COLOR_PAIR(this->color));

    win->refresh();

}

void CellCursor::constraintCursor()
{
    this->max_x = this->grid->getWidth() - this->cursor_width;
    this->max_y = this->grid->getHeight() - this->cursor_height;

    if(x < 0)
        x = 0;
    if(y < 0)
        y = 0;
    if(x > max_x)
        x = max_x;
    if(y > max_y)
        y = max_y;


    this->prev_x = this->x;
    this->prev_y = this->y;
}

void CellCursor::init()
{
    this->x = this->prev_x = 0;
    this->y = this->prev_y = 0;

    this->max_x = this->grid->getWidth() - this->cursor_width;
    this->max_y = this->grid->getHeight() - this->cursor_height;
}
UpdateState CellCursor::update(chtype ch)
{
    if(!visible)
        return SUCCESS;

    prev_x = x;
    prev_y = y;
    switch(ch)
    {
    case KEY_UP:
        y--;
        break;
    case KEY_DOWN:
        y++;
        break;
    case KEY_LEFT:
        x--;
        break;
    case KEY_RIGHT:
        x++;
        break;
    case 27:
        if(selectionCanceled_callback != NULL)
        {
            selectionCanceled_callback();
            return REMOVE;
        }
        break;
    case 10:
        if(cellSelected_callback != NULL && cellSelected_callback(x, y))
            return REMOVE;
        break;
    default:
        // empty
        break;
    }

    this->max_x = this->grid->getWidth() - this->cursor_width;
    this->max_y = this->grid->getHeight() - this->cursor_height;

    if(x < 0)
        x = 0;
    if(y < 0)
        y = 0;
    if(x > max_x)
        x = max_x;
    if(y > max_y)
        y = max_y;

    if(x != max_x || y != max_y)
        return OVERRIDE;
    return SUCCESS;
}
void CellCursor::render()
{
    if(!visible)
        return;
    if(x == prev_x && y == prev_y)
        return;
    drawBox();
}
