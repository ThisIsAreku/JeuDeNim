#include <iostream>
#include <sstream>
#include <thread>

#include "Game.h"

#include "widgets/CellCursor.h"
#include "entities/Random.h"
#include "entities/Humain.h"
#include "providers/DefaultGravityProvider.h"



#include "Logger.h"

// definition of Logger::log
Logger::Logger Logger::log;

Game::Game()
{
    Logger::log << "Game start" << std::endl;

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

    if(!interrupted)
    {
        for(int i = 0; i < getGameSettings()->getNumAlign(); i++)
        {
            TokenAlignement al = getWinnerChecker()->getWinAlignement(currentPlayer)[i];
            this->tokenLiner->displayAlignement(al);
            Logger::log << "Winner align is " << al.x1 << "," << al.y1 << " -> " << al.x2 << "," << al.y2 << " (type " << al.orientation << ")" << std::endl;
        }

        Window *win = getWindowManager()->getWindow(WIN_GAME_TURN);
        if(win == NULL)
            return;

        win->clear();
        win->printAt(0, 0, "Partie terminée");
        win->printAt(0, 1, "Appuyez sur [ESPACE] pour quitter");
        win->refresh();
        while(getch() != 32) { };
    }


    deinit();

}

void Game::init()
{
    this->grid = new Grid(this->manager, WIN_GAME_GRID, this);
    this->gravityProvider = new DefaultGravityProvider(this->grid);
    this->winnerChecker = new WinnerChecker(this, true);
    this->tokenLiner = new TokenLiner(this->manager, WIN_GAME_GRID);

    this->players = new Entity*[getGameSettings()->getNumPlayers()];

    for(int i = 0; i < getGameSettings()->getNumPlayers(); ++i)
    {
        switch(getGameSettings()->getPlayerTypes()[i])
        {
        case ENTITY_HUMAIN:
            this->players[i] = new Humain(this, i + 1);
            break;
        case ENTITY_DUMBASS:
            this->players[i] = new Random(this, i + 1);
            break;
        case ENTITY_AI:
            //this->players[i] = new Random(this, i + 1);
            break;
        default:
            exit(-1);
        }
    }

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
    delete this->tokenLiner;
}

void Game::update()
{
    if(getCurrentPlayer()->getEntityType() != ENTITY_HUMAIN)
        timeout(50);
    else
        timeout(-1);
    chtype ch = getch();
    logKeyboard(ch);

    if(getCurrentPlayer()->getEntityType() == ENTITY_HUMAIN)
    {
        if((int)ch == -1)
            return;
    }

    doKeyboardActions(ch);

    this->grid->update(ch);
    getCurrentPlayer()->update(ch);
}

void Game::render()
{
    getWindowManager()->print(WIN_GAME_TURN, 0, 0, "(F9) Animations: ");
    if(getGameSettings()->animate)
        getWindowManager()->print(WIN_GAME_TURN, 17, 0, "ON ");
    else
        getWindowManager()->print(WIN_GAME_TURN, 17, 0, "OFF");


    getWindowManager()->print(WIN_GAME_TURN, 0, 1, "Joueur actuel: ");
    getWindowManager()->printInt(WIN_GAME_TURN, 15, 1, currentPlayer + 1);
    const char *playerId = getCurrentPlayer()->getId();
    getWindowManager()->print(WIN_GAME_TURN, 16, 1, " (");
    getWindowManager()->print(WIN_GAME_TURN, 18, 1, playerId);
    getWindowManager()->print(WIN_GAME_TURN, 18 + strlen(playerId), 1, ")                  ");

    getWindowManager()->refreshWindow(WIN_GAME_TURN);

    getCurrentPlayer()->render();

    this->grid->render();

}

void Game::doKeyboardActions(chtype ch)
{
    if(ch == KEY_F(12))
        interrupted = true;
    if(ch == KEY_F(9))
        getGameSettings()->animate = !getGameSettings()->animate;
}

void Game::invokeEntityTurn(int n)
{
    Logger::log << "Game: currentPlayer = " << n << std::endl;
    currentPlayer = n;
    getCurrentPlayer()->turn();
}


bool Game::onEntityTurnCompleted(EntityTurnAction action, int x, int y)
{
    std::ostringstream oss;
    oss << "[" << currentPlayer + 1 << "] ";
    if(action == TOKEN_PLACE)
    {
        if(!getBaseGrid()->placeToken(currentPlayer + 1, x))
            return false;
        oss << "++ @ " << x;
    }
    else if(action == TOKEN_REMOVE)
    {
        if(!getBaseGrid()->removeToken(x, y))
            return false;
        oss << "-- @ " << x << "," << y;
    }
    else
    {
        getBaseGrid()->rotate(action);
        oss << "Rt @ ";
        if(action == ROTATE_CLOCKWISE)
        {
            oss << "Left";
        }
        else
        {
            oss << "Right";
        }
    }
    appendToPlayTurns(oss.str().c_str());
    if(getWinnerChecker()->hasWinner())
    {
        oss.str("");
        currentPlayer = getWinnerChecker()->getWinnerId();
        oss << "[" << currentPlayer + 1 << "] WIN !";
        appendToPlayTurns(oss.str().c_str());

        Logger::log << "onEntityTurnCompleted: win for " << currentPlayer + 1 << std::endl;
        game_end = true;
    }
    else
        invokeEntityTurn(++currentPlayer % getGameSettings()->getNumPlayers());
    return true;

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
    if((int)ch == -1)
        return;
    Logger::log << ch << " :: ";
    unsigned long longVal = static_cast<unsigned long>(ch);

    if(265 <= longVal && longVal <= 276)
    {
        Logger::log << "[F" << (longVal - 264) << "]";
    }
    else
        switch(longVal)
        {
        case 32:
            Logger::log << "[SPACE]";
            break;
        case 10:
            Logger::log << "[ENTER]";
            break;

        case 258:
            Logger::log << "[DOWN]";
            break;
        case 259:
            Logger::log << "[UP]";
            break;
        case 260:
            Logger::log << "[LEFT]";
            break;
        case 261:
            Logger::log << "[RIGHT]";
            break;
        default:
            Logger::log << static_cast<char>(ch & A_CHARTEXT);
        }
    Logger::log << std::endl;
}

/***********/
/* PUBLICS */
/***********/
void Game::start()
{
    if(COLS < 80 || LINES < 24)
    {
        this->manager->leaveCurseMode();
        std::cout << "Non, là c'est vraiment trop petit !" << std::endl << "La taille compte un peu, quand même..." << std::endl;
        return;
    }
    this->manager->initialize("JeuDeNim v0.1");

    Window *win = getWindowManager()->getWindow(WIN_GAME_GRID);
    if(win == NULL)
        return;

    Logger::log << "COLS: " << COLS << ", LINES: " << LINES << std::endl;

    int maxWidth = (int)(((6 * COLS / 8.) - 2 - 2) / CELL_WIDTH) - 1;
    int maxHeight = (int)(((LINES - 6) - 1 - 2) / CELL_HEIGHT) - 1;
    int maxSize = maxWidth;
    if(maxSize > maxHeight)
        maxSize = maxHeight;

    int dispLine = -1;

    win->printAt(0, ++dispLine, "Avec la taille de votre fenêtre, vous pouvez utiliser au maximum");
    win->printAt(0, ++dispLine, "une grille de taille");

    win->AttribOn(COLOR_PAIR(30));
    getWindowManager()->printInt(WIN_GAME_GRID, 21, dispLine, maxSize);
    win->AttribOff(COLOR_PAIR(30));

    win->printAt(24, dispLine, ".Vous pouvez utiliser une grille");
    win->printAt(0, ++dispLine, "plus grande, mais l'affichage souffre de quelques problèmes");
    win->printAt(0, ++dispLine, "pour les grandes grilles...");

    dispLine++;
    win->refresh();

    getGameSettings()
    ->input(win, ++dispLine)
    ->commit();

    win->clear();
    win->refresh();

    interrupted = false;
    loop();
}