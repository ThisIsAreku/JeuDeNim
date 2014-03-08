#include <iostream>
#include <sstream>
#include <thread>
#include <algorithm>

#include "Game.h"

#include "widgets/CellCursor.h"
#include "entities/Humain.h"

Game::Game()
{

    this->manager = new WindowManager();
    this->gameSettings = new GameSettings();

    this->players = NULL;
    playTurnIndex = 0;
    currentPlayer = 0;
}
Game::~Game()
{
    if(this->players != NULL)
        delete [] this->players;

    delete this->gameSettings;
    delete this->manager;
}

WindowManager *Game::getWindowManager()
{
    return this->manager;
}

Entity *Game::getCurrentPlayer()
{
    return this->players[currentPlayer];
}
BaseGrid *Game::getBaseGrid()
{
    return this->grid;
}
GameSettings *Game::getGameSettings()
{
    return this->gameSettings;
}


void Game::loop()
{
    init();
    render();
    while(!interrupted)
    {
        update();

        render();
    }
    deinit();
}

void Game::init()
{
    this->grid = new Grid(this->manager, WIN_GAME_GRID, getGameSettings());
    this->players = new Entity*[getGameSettings()->getNumPlayers()];

    for(int i = 0; i < getGameSettings()->getNumPlayers(); ++i)
        this->players[i] = new Humain(this, i + 1);

    this->grid->init();
    getCurrentPlayer()->init();
    invokeEntityTurn(0);
}
void Game::deinit()
{
    for(int i = 0; i < getGameSettings()->getNumPlayers(); ++i)
        delete this->players[i];

    delete this->grid;
}

void Game::update()
{
    timeout(1000);
    chtype ch = getch();
    if((int)ch == -1)
        return;

    getWindowManager()->appendInt(WIN_GAME_TURN, ch);
    getWindowManager()->append(WIN_GAME_TURN, " ");
    getWindowManager()->append(WIN_GAME_TURN, ch);
    getWindowManager()->append(WIN_GAME_TURN, " - ");
    getWindowManager()->refreshWindow(WIN_GAME_TURN);

    if(ch == KEY_F(12))
        interrupted = true;

    this->grid->update(ch);
    getCurrentPlayer()->update(ch);
}

void Game::render()
{
    getCurrentPlayer()->render();

    this->grid->render();

}

void Game::invokeEntityTurn(int n)
{
    currentPlayer = n;
    getCurrentPlayer()->turn();
}


bool Game::onEntityTurnCompleted(EntityTurnAction action, int x, int y)
{
    std::ostringstream oss;
    bool valid = true;
    if(action == TOKEN_PLACE)
    {
        valid = getBaseGrid()->placeToken(currentPlayer + 1, x);
        if(valid)
        {
            oss << "Joueur " << currentPlayer + 1 << " a joué dans la colonne " << x;
        }
    }
    else if(action == TOKEN_REMOVE)
    {
        valid = getBaseGrid()->removeToken(x, y);
        if(valid)
        {
            oss << "Joueur " << currentPlayer + 1 << " a supprimé un jeton (" << x << "," << y << ")";
        }
        //this->grid->doGravity();
    }
    else
    {

        oss << "Joueur " << currentPlayer + 1 << " a retourné le plateau vers la ";
        if(action == ROTATE_CLOCKWISE)
        {
            oss << "droite";
        }
        else
        {
            oss << "gauche";
        }
        getBaseGrid()->rotate(action);
        //this->grid->doGravity();
    }
    if(valid)
    {
        appendToPlayTurns(oss.str().c_str());
        invokeEntityTurn(++currentPlayer % getGameSettings()->getNumPlayers());
    }
    return valid;

}
void Game::appendToPlayTurns(const char *s)
{
    Window *win = getWindowManager()->getWindow(WIN_SCOREBOARD);
    if(win == NULL)
        return;


    if(playTurnIndex > win->getHeight() - 3)
    {
        win->clear();
        playTurnIndex = 0;
    }
    int lastPlayTurnIndex = playTurnIndex - 1;
    if(lastPlayTurnIndex == -1)
    {
        win->setChar(0, win->getHeight() - 3, ' ');

    }
    else
    {
        win->setChar(0, lastPlayTurnIndex, ' ');
    }

    win->setChar(0, playTurnIndex, ACS_RARROW);
    win->setChar(1, playTurnIndex, ' ');

    win->AttribOn(COLOR_PAIR(currentPlayer + 9));
    win->printAt(2, playTurnIndex++, s);
    win->AttribOff(COLOR_PAIR(currentPlayer + 9));
    win->refresh();
}

/***********/
/* PUBLICS */
/***********/
void Game::start()
{
    this->manager->initialize("JeuDeNim v0.1");
    getGameSettings()->commit();
    /*
        Window *win = getWindowManager()->getWindow(WIN_GAME_GRID);
        if(win == NULL)
            return;

        int maxSize = fmin((int)((COLS - 5 - 12) / 4), (int)((LINES - 4 - 6) / 2));
        win->printAt(0, 0, "Avec la taille de votre fenêtre, vous pouvez utiliser au maximum");
        win->printAt(0, 1, "une grille de taille");

        win->AttribOn(COLOR_PAIR(30));
        getWindowManager()->printInt(WIN_GAME_GRID, 21, 1, maxSize);
        win->AttribOff(COLOR_PAIR(30));
        win->printAt(0, 2, "Vous pouvez utiliser une grille plus grande, mais l'affichage");
        win->printAt(0, 3, "souffre de quelques problèmes pour les grandes grilles...");

        win->printAt(0, 5, "Entrez la largeur de la grille: ");
        win->refresh();
        do
        {
            win->readAnyAt(31, 5, "%d", &initGridW);
        }
        while(initGridW < 2);
        win->printAt(0, 6, "Entrez la hauteur de la grille: ");
        win->refresh();
        do
        {
            win->readAnyAt(31, 6, "%d", &initGridH);
        }
        while(initGridH < 2);

        win->printAt(0, 8, "Entrez le nombre de joueurs (2-4): ");
        win->refresh();
        do
        {
            win->readAnyAt(35, 8, "%d", &getGameSettings()->getNumPlayers());
        }
        while(getGameSettings()->getNumPlayers() < 2 || getGameSettings()->getNumPlayers() > 4);

        win->clear();
    */
    interrupted = false;
    loop();
}