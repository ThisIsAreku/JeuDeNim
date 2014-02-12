#include "Game.h"

Game::Game()
{
    this->grid = new Grid(10, 10);
    this->manager = new WindowManager();
}
Game::~Game()
{
    delete this->grid;
    delete this->manager;
}


void Game::loop()
{
    this->manager->initialize("JeuDeNim v0.1");
    init();
    while(!interrupted)
    {
        this->manager->update();
        render();

        update();

    }
}

void Game::init()
{
    this->manager->printBaseGrid(WIN_GAME_GRID, this->grid, 5, 5);
}

void Game::update()
{
    int ch = getch();
    if(ch == KEY_F(2))
        interrupted = true;
    if(ch == KEY_F(5)){
        this->manager->refreshAll();
        this->manager->printBaseGrid(WIN_GAME_GRID, this->grid, 5, 5);
    }
}

void Game::render()
{
}




/***********/
/* PUBLICS */
/***********/
void Game::start()
{
    interrupted = false;
    loop();
}
void Game::displayGrid()
{
    grid->setGridAt(0, 0, 1);
    grid->setGridAt(1, 0, 1);
    grid->setGridAt(2, 0, 1);
    grid->setGridAt(3, 0, 1);
    grid->setGridAt(4, 0, 2);
    grid->setGridAt(0, 1, 2);
    grid->setGridAt(0, 2, 1);
    grid->setGridAt(0, 3, 2);
    this->manager->printGrid(WIN_GAME_GRID, grid, 2, 2);
}
void Game::wait()
{
    int ch;
    while((ch = getch()) != KEY_F(2))
    {
        if(ch == KEY_F(5))
        {

        }
    }
}