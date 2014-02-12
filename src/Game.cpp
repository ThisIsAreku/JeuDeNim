#include "Game.h"

Game::Game()
{
    this->grid = new Grid(5, 5);
    this->manager = new WindowManager();
}
Game::~Game()
{
    delete this->grid;
    delete this->manager;
}




/***********/
/* PUBLICS */
/***********/
void Game::displayGrid()
{
    this->manager->printGrid(WIN_GAME_GRID, grid);
    /*this->manager->print(WIN_GAME_GRID, 1, 1, "Hello");
    this->manager->print(WIN_GAME_TURN, 1, 1, "Entre un truc:");
    this->manager->print(WIN_SCOREBOARD, 1, 1, "....");
    char c[256];
    if(this->manager->read(WIN_GAME_TURN, 2, 1, c))
    {
        if(c == NULL)
        {
            this->manager->print(WIN_SCOREBOARD, 5, 1, "NULL !");
        }
        else
        {
            this->manager->print(WIN_SCOREBOARD, 5, 1, c);
        }
    }
    else
    {
        this->manager->print(WIN_SCOREBOARD, 5, 1, "Erreur");
    }
    /*char str[80];
    getstr(str);
    WINDOW* win = getWindow(WIN_SCOREBOARD);
    mvwprintw(win, 0, 0, "You Entered: %s", str);
    wrefresh(win);
    int ch;
    while((ch = getch()) != KEY_F(2))
    {
    }*/
}
void Game::wait()
{
    int ch;
    while((ch = getch()) != KEY_F(2))
    {
    }
}