#include <iostream>
#include <sstream>
#include <thread>

#ifdef __linux__
#include <sys/stat.h>
#else
#endif

#include "Game.h"

#include "widgets/CellCursor.h"
#include "entities/Random.h"
#include "entities/Humain.h"
#include "entities/AI.h"
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
    game_end = turn_end = false;
    displayingHelp = false;
    random_testing = false;

#ifdef __linux__
    char *tmp = getenv("HOME");
    saveFilePath = new char[strlen(tmp) + 12];

    strncpy(saveFilePath, tmp, strlen(tmp));
    strncpy(saveFilePath + strlen(tmp), "/.jeudenim/", 12);

    mkdir(saveFilePath, 0777);
#else
    saveFilePath = new char[3];
    strncpy(saveFilePath, "./", 3);
#endif
    Logger::log << "Save file path is : " << saveFilePath << std::endl;
}
Game::~Game()
{
    delete this->gameSettings;
    delete this->manager;
    delete [] saveFilePath;
}
bool Game::isRandomTesting()
{
    return this->random_testing;
}
void Game::setRandomTesting(bool rnd)
{
    this->random_testing = rnd;
}

WindowManager *Game::getWindowManager()
{
    return this->manager;
}

Entity *Game::getCurrentPlayer()
{
    return this->players[currentPlayer];
}
DisplayGrid *Game::getDisplayGrid()
{
    return this->displayGrid;
}
Grid *Game::getGrid()
{
    return this->grid;
}
GameSettings *Game::getGameSettings()
{
    return this->gameSettings;
}
WinnerChecker *Game::getWinnerChecker()
{
    return this->winnerChecker;
}


void Game::loop()
{
    tick = 0;
    init();
    render();
    while(!interrupted && !game_end)
    {
        tick++;
        update();

        render();
        if(tick % 10 == 0)
            Logger::log << "Tick " << tick << std::endl;
    }

    if(!interrupted)
    {

        for(int n = 0; n < getWinnerChecker()->getNumWinner(); n++)
        {
            currentPlayer = getWinnerChecker()->getWinnerId(n);
            for(int i = 0; i < getGameSettings()->getNumAlign(); i++)
            {
                TokenAlignement al = getWinnerChecker()->getWinAlignement(currentPlayer)[i];
                this->tokenLiner->displayAlignement(al);
                Logger::log << "Winner align is " << al.x1 << "," << al.y1 << " -> " << al.x2 << "," << al.y2 << " (type " << al.orientation << ")" << std::endl;
            }
        }

        Window *win = getWindowManager()->getWindow(WIN_GAME_TURN);
        if(win == NULL)
            return;

        win->clear();
        win->printAt(0, 0, "Partie terminée");
        win->printAt(0, 1, "Appuyez sur [ESPACE] pour quitter");
        win->refresh();
        if(isRandomTesting())
        {
            timeout(500);
            getch();
            timeout(0);
        }
        else
            while(getch() != 32) { };
    }


    deinit();

}

void Game::init()
{
    this->grid = new Grid(getGameSettings()->getBoardWidth(), getGameSettings()->getBoardHeight());

    this->displayGrid = new DisplayGrid(this->manager, WIN_GAME_GRID, this);
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
            this->players[i] = new AI(this, i + 1, getGameSettings()->getAILevels()[i]);
            break;
        default:
            exit(-1);
        }
        this->players[i]->init();
    }

    this->displayGrid->init();
    invokeEntityTurn(0);
}
void Game::deinit()
{
    for(int i = 0; i < getGameSettings()->getNumPlayers(); ++i)
        delete this->players[i];
    delete [] this->players;

    delete this->displayGrid;
    delete this->winnerChecker;
    delete this->tokenLiner;

    delete this->grid;
}

void Game::update()
{
    if(getCurrentPlayer()->getEntityType() != ENTITY_HUMAIN)
        timeout(50);
    else
        timeout(-1);
    chtype ch = static_cast<chtype>(getch());
    logKeyboard(ch);

    if(getCurrentPlayer()->getEntityType() == ENTITY_HUMAIN)
    {
        if(static_cast<int>(ch) == -1)
            return;
    }

    doKeyboardActions(ch);

    if(!displayingHelp)
    {
        this->displayGrid->update(ch);
        getCurrentPlayer()->update(ch);
    }
    if(turn_end)
    {
        invokeEntityTurn(++currentPlayer % getGameSettings()->getNumPlayers());
    }
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
    getWindowManager()->print(WIN_GAME_TURN, 18 + static_cast<int>(strlen(playerId)), 1, ")                  ");

    getWindowManager()->refreshWindow(WIN_GAME_TURN);


    if(!displayingHelp)
    {
        getCurrentPlayer()->render();

        this->displayGrid->render();
    }

}

void Game::renderOps()
{
    double p = getCurrentPlayer()->getOperationPercent();
    if(p == 100)
    {
        getWindowManager()->print(WIN_GAME_TURN, -7, 1, "       ");
        getWindowManager()->print(WIN_GAME_TURN, -18, 0, "                   ");
        getWindowManager()->refreshWindow(WIN_GAME_TURN);
        return;
    }
    if(p == 0)
    {
        getWindowManager()->print(WIN_GAME_TURN, -18, 0, "Calcul en cours...");
    }

    int i = 0;
    if(p < 10)
        i = 1;
    else if(p < 100)
        i = 2;
    else if(p == 100)
        i = 3;

    char str[6];
    sprintf(str, "%.2f", p);
    getWindowManager()->print(WIN_GAME_TURN, -4 - i, 1, str);
    getWindowManager()->print(WIN_GAME_TURN, -1, 1, "%%");


    getWindowManager()->refreshWindow(WIN_GAME_TURN);
}

void Game::doKeyboardActions(chtype ch)
{
    if(ch == KEY_F(12))
        interrupted = true;
    if(ch == 'h')
        displayHelp();

    if(displayingHelp)
        return;
    if(ch == KEY_F(9))
        getGameSettings()->animate = !getGameSettings()->animate;
    if(ch == KEY_F(6))
        saveState(0);
    if(ch == KEY_F(7))
        restoreState(0);
}

void Game::invokeEntityTurn(int n)
{
    Logger::log << "Game: currentPlayer = " << n << std::endl;
    currentPlayer = n;
    turn_end = false;
    getCurrentPlayer()->turn();
}


bool Game::onEntityTurnCompleted(EntityTurnAction action, int x, int y)
{
    Logger::log << "Game: onEntityTurnCompleted for " << currentPlayer << " (" << action << ", " << x << ", " << y << ")" << std::endl;
    std::ostringstream oss;
    oss << "[" << currentPlayer + 1 << "] ";
    if(action == TOKEN_PLACE)
    {
        if(!getDisplayGrid()->placeToken(currentPlayer + 1, x))
            return false;
        oss << "++ @ " << x;
    }
    else if(action == TOKEN_REMOVE)
    {
        if(getGrid()->getGridAt(x, y) != currentPlayer + 1)
            return false;
        if(!getDisplayGrid()->removeToken(x, y))
            return false;
        oss << "-- @ " << x << "," << y;
    }
    else
    {
        Rotation rotate = static_cast<Rotation>(x);
        getDisplayGrid()->rotate(rotate);
        oss << "Rt @ ";
        if(rotate == ROTATE_CLOCKWISE)
        {
            oss << "Left";
        }
        else
        {
            oss << "Right";
        }
    }
    getWinnerChecker()->searchWinner(getGrid());
    appendToPlayTurns(oss.str().c_str());
    if(getWinnerChecker()->hasWinner() || konamiStep == 10)
    {
        oss.str("");
        if(getWinnerChecker()->getNumWinner() != getGameSettings()->getNumPlayers())
        {
            if(konamiStep != 10)
                currentPlayer = getWinnerChecker()->getWinnerId(0);
            oss << "[" << currentPlayer + 1 << "] WIN !";
        }
        else
        {
            oss << "EGALITE !";
        }
        appendToPlayTurns(oss.str().c_str());

        Logger::log << "onEntityTurnCompleted: win for " << currentPlayer + 1 << std::endl;
        if(konamiStep == 10)
            Logger::log << "Win by Konami !" << std::endl;
        game_end = true;
    }
    else
    {
        turn_end = true;
    }
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
    if(static_cast<int>(ch) == -1)
        return;
    Logger::log << ch << " :: ";
    unsigned long longVal = static_cast<unsigned long>(ch);
    if(konamiStep != 10)
    {
        if(konami[konamiStep] == longVal)
            konamiStep++;
        else
            konamiStep = 0;
    }

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

void Game::displayHelp()
{
    displayingHelp = !displayingHelp;
    if(!displayingHelp)
        return;


    Window *win = getWindowManager()->getWindow(WIN_HELP);
    if(win == NULL)
        return;

    win->clear();
    win->printAt(0, 0, "Donc là c'est l'aide...");

    int i = 2;
    win->printAt(0, i++, "Touches d'actions");
    win->printAt(0, i++, "- P: Placer un jeton");
    win->printAt(0, i++, "- D: Supprimer un jeton");
    win->printAt(0, i++, "- R: Rotation antihoraire");
    win->printAt(0, i++, "- T: Rotation horaire");

    i = 2;
    win->printAt(34, i++, "Déplacer le plateau");
    win->printAt(34, i++, "- O: Haut");
    win->printAt(34, i++, "- L: Bas");
    win->printAt(34, i++, "- K: Gauche");
    win->printAt(34, i++, "- M: Droite");
    i++;
    win->printAt(0, i, "Appuyez sur F6 pour sauvegarder, F7 pour charger");


    win->printAt(0, -1, "Appuyez sur [H] pour fermer l'aide");

    win->refresh();


    overlay(getWindowManager()->getWindow(WIN_GAME_GRID)->getHandle(), win->getHandle());
    refresh();
}


void Game::saveState(int slotId)
{
    if(slotId < 0 || slotId > 9)
        return;
    char *thisSaveFile = new char[strlen(saveFilePath) + strlen(saveFileName) + 1];

    strcpy(thisSaveFile, saveFilePath);
    strcpy(thisSaveFile + strlen(saveFilePath), saveFileName);

    thisSaveFile[strlen(thisSaveFile) - 1] = '0' + static_cast<char>(slotId);
    Logger::log << "Saving to " << thisSaveFile << std::endl;

    std::ofstream saveFile(thisSaveFile, std::ofstream::out | std::ofstream::trunc);
    if(saveFile)
    {

        saveFile << currentPlayer << std::endl << *getGameSettings() << *getGrid();
        saveFile << "ALL YOUR BASE ARE BELONG TO US";
    }
    delete [] thisSaveFile;
}

void Game::restoreState(int slotId)
{
    if(slotId < 0 || slotId > 9)
        return;
    char *thisSaveFile = new char[strlen(saveFilePath) + strlen(saveFileName) + 1];

    strcpy(thisSaveFile, saveFilePath);
    strcpy(thisSaveFile + strlen(saveFilePath), saveFileName);

    thisSaveFile[strlen(thisSaveFile) - 1] = '0' + static_cast<char>(slotId);
    Logger::log << "Loading from " << thisSaveFile << std::endl;

    std::ifstream saveFile(thisSaveFile, std::ofstream::in);
    if(saveFile)
    {
        for(int i = 0; i < getGameSettings()->getNumPlayers(); ++i)
            delete this->players[i];
        delete [] this->players;

        saveFile >> currentPlayer >> *getGameSettings() >> *getGrid();

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
                this->players[i] = new AI(this, i + 1, getGameSettings()->getAILevels()[i]);
                break;
            default:
                exit(-1);
            }
            this->players[i]->init();
        }

        getWindowManager()->getWindow(WIN_GAME_GRID)->clear();
        getWindowManager()->getWindow(WIN_SCOREBOARD)->clear();
        playTurnIndex = 0;
        getDisplayGrid()->init();
        invokeEntityTurn(currentPlayer);
    }
    delete [] thisSaveFile;
}

/***********/
/* PUBLICS */
/***********/
void Game::start()
{
    konamiStep = 0;
    if(COLS < 80 || LINES < 24)
    {
        getWindowManager()->leaveCurseMode();
        std::cout << "Non, là c'est vraiment trop petit !" << std::endl << "La taille compte un peu, quand même..." << std::endl;
        return;
    }
    getWindowManager()->initialize("JeuDeNim v0.1");

    Window *win = getWindowManager()->getWindow(WIN_GAME_GRID);
    if(win == NULL)
        return;

    Logger::log << "COLS: " << COLS << ", LINES: " << LINES << std::endl;

    int maxWidth = static_cast<int>((((COLS  - 19) - 2 - 2) / CELL_WIDTH) - 1);
    int maxHeight = static_cast<int>((((LINES - 5) - 1 - 2) / CELL_HEIGHT) - 1);
    int maxSize = maxWidth;
    if(maxSize > maxHeight)
        maxSize = maxHeight;

    int dispLine = -1;

    win->printAt(0, ++dispLine, "Avec ce terminal, vous pouvez utiliser au maximum");
    win->printAt(0, ++dispLine, "une grille de taille");

    win->AttribOn(COLOR_PAIR(30));
    getWindowManager()->printInt(WIN_GAME_GRID, 21, dispLine, maxSize);
    win->AttribOff(COLOR_PAIR(30));

    win->printAt(24, dispLine, ".Vous pouvez utiliser une grille");
    win->printAt(0, ++dispLine, "plus  grande, mais  l'affichage  souffre  de  quelques");
    win->printAt(0, ++dispLine, "problèmes pour les grandes grilles...");

    dispLine++;
    win->refresh();

    if(!isRandomTesting())
    {

        getGameSettings()
        ->input(win, ++dispLine)
        ->commit();

    }
    else
    {

        getGameSettings()
        ->setNumAlign(3)
        ->setPlayerType(0, ENTITY_DUMBASS)
        ->setPlayerType(1, ENTITY_DUMBASS)
        ->commit();
    }

    win->clear();
    win->refresh();

    interrupted = false;
    while(!interrupted)
    {
        playTurnIndex = 0;
        currentPlayer = 0;
        game_end = turn_end = false;
        loop();
        if(!isRandomTesting())
            break;
    }

}
