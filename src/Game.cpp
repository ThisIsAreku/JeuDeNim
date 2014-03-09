#include <iostream>
#include <sstream>
#include <thread>
#include <algorithm>

#include "Game.h"

#include "widgets/CellCursor.h"
#include "entities/Humain.h"
#include "providers/DefaultGravityProvider.h"

Game::Game()
{
    this->manager = new WindowManager();
    this->gameSettings = new GameSettings();

    this->players = NULL;
    playTurnIndex = 0;
    currentPlayer = 0;
    game_end = false;
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
GravityProvider *Game::getGravityProvider()
{
    return this->gravityProvider;
}
WinnerChecker *Game::getWinnerChecker()
{
    return this->winnerChecker;
}


void Game::loop()
{
    init();
    render();
    while(!interrupted && !game_end)
    {
        update();

        render();
    }
    deinit();

    if(!interrupted)
    {
        Window *win = getWindowManager()->getWindow(WIN_GAME_GRID);
        if(win == NULL)
            return;

        win->printAt_unshifted(0, 5, "Partie terminée");
        win->printAt_unshifted(0, 6, "Appuyez sur une touche pour quitter");
        win->refresh();
        getch();
    }

}

void Game::init()
{
    this->grid = new Grid(this->manager, WIN_GAME_GRID, this);
    this->gravityProvider = new DefaultGravityProvider(this->grid);
    this->winnerChecker = new WinnerChecker(this);

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
    delete this->gravityProvider;
    delete this->winnerChecker;
}

void Game::update()
{
    chtype ch = getch();

    logKeyboard(ch);

    if((int)ch == -1)
        return;

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
    std::cerr << "Game: currentPlayer = " << n << std::endl;
    currentPlayer = n;
    getCurrentPlayer()->turn();
}


bool Game::onEntityTurnCompleted(EntityTurnAction action, int x, int y)
{
    std::ostringstream oss;
    oss << "[" << currentPlayer + 1 << "] ";
    bool valid = true;
    if(action == TOKEN_PLACE)
    {
        valid = getBaseGrid()->placeToken(currentPlayer + 1, x);
        if(valid)
        {
            oss << "++ @ " << x;
        }
    }
    else if(action == TOKEN_REMOVE)
    {
        valid = getBaseGrid()->removeToken(x, y);
        if(valid)
        {
            oss << "-- @ " << x << "," << y;
        }
    }
    else
    {
        oss << "R @ ";
        if(action == ROTATE_CLOCKWISE)
        {
            oss << "Left";
        }
        else
        {
            oss << "Right";
        }
        getBaseGrid()->rotate(action);
    }
    if(valid)
    {
        appendToPlayTurns(oss.str().c_str());
        if(getWinnerChecker()->getWinnerId() != -1)
        {
            oss.str("");
            currentPlayer = getWinnerChecker()->getWinnerId();
            oss << "[" << currentPlayer + 1 << "] WIN !";
            appendToPlayTurns(oss.str().c_str());
            std::cerr << "onEntityTurnCompleted: win for " << currentPlayer + 1 << std::endl;
            game_end = true;
        }
        else
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

void Game::logKeyboard(chtype ch)
{

    std::cerr << ch << " :: ";
    unsigned long longVal = static_cast<unsigned long>(ch);

    if(265 <= longVal && longVal <= 276)
    {
        std::cerr << "[F" << (longVal - 264) << "]";
    }
    else
        switch(longVal)
        {
        case 32:
            std::cerr << "[SPACE]";
            break;
        case 10:
            std::cerr << "[ENTER]";
            break;

        case 258:
            std::cerr << "[DOWN]";
            break;
        case 259:
            std::cerr << "[UP]";
            break;
        case 260:
            std::cerr << "[LEFT]";
            break;
        case 261:
            std::cerr << "[RIGHT]";
            break;
        default:
            std::cerr << static_cast<char>(ch & A_CHARTEXT);
        }
    std::cerr << std::endl;
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