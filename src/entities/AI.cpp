#include "entities/AI.h"

#ifdef _REENTRANT
#include <thread>
#endif
#include <ctime>
#include <cmath>
#include "Logger.h"

AI::AI(Game *game, int entityIndex, int level, bool adaptative) : super(game, entityIndex)
{
#ifdef _REENTRANT
    this->winnerChecker = new WinnerChecker[3];
    for (int i = 0; i < 3; ++i)
    {
        this->winnerChecker[i].init(game, false);
    }
#else
    this->winnerChecker = new WinnerChecker(game, false);
#endif
    this->adaptative = adaptative;
    this->totalEvalOpsByLevel = NULL;
    setDifficulty(level);
    if(adaptative)
        Logger::log << "(" << entityIndex << ") Tree power ACTIVATE!" << std::endl;
}
AI::~AI()
{
    delete this->winnerChecker;
    delete [] totalEvalOpsByLevel;
}

void AI::setDifficulty(int difficulty)
{
    if(difficulty > 15)
        difficulty = 15;
    else if(difficulty < 1)
        difficulty = 1;
    this->difficulty = difficulty;
    if(totalEvalOpsByLevel != NULL)
        delete [] totalEvalOpsByLevel;
    totalEvalOpsByLevel = new long[difficulty];
}

int AI::getDifficulty()
{
    return this->difficulty;
}
bool AI::isAdaptative()
{
    return this->adaptative;
}

float AI::getOperationPercent() const
{
    return (currentEvalOps * 100) / static_cast<float>(totalEvalOps);
}

void AI::startAIComputation()
{
    // cols + delete + rotate
    numPossibles = getGame()->getGrid()->getWidth() + getGame()->getGrid()->getTotalCells() + 2;
    totalEvalOps = static_cast<long>(pow(numPossibles, difficulty));
    for(int i = 0; i < difficulty; ++i)
        totalEvalOpsByLevel[i] = static_cast<long>(pow(numPossibles, i));

    turn_choice.valid = false;

    currentEvalOps = 0;
    Logger::log << "numPossibles: " << numPossibles << std::endl;
    Logger::log << "totalEvalOps: " << totalEvalOps << std::endl;

    getGame()->renderOps();

    IATurnChoice placeChoice, removeChoice, rotateChoice;
#ifdef _REENTRANT
    Logger::log << "Computation using 3 threads" << std::endl;
    std::thread computationPlaceThread(&AI::doComputationPlace, this, &placeChoice, 0);
    std::thread computationRemoveThread(&AI::doComputationRemove, this, &removeChoice, 1);
    std::thread computationRotateThread(&AI::doComputationRotate, this, &rotateChoice, 2);

    computationPlaceThread.join();
    computationRemoveThread.join();
    computationRotateThread.join();

#else
    Logger::log << "Computation on a single thread" << std::endl;
    /****************************/
    /******* PLACE_TOKEN *******/
    /**************************/
    doComputationPlace(&placeChoice);

    /****************************/
    /******* REMOVE_TOKEN *******/
    /**************************/
    doComputationRemove(&removeChoice);

    /***************************/
    /********* ROTATE **********/
    /***************************/
    // ROTATE_CLOCKWISE = 1
    // ROTATE_COUNTERCLOCKWISE = -1
    doComputationRotate(&rotateChoice);
#endif




    Logger::log << "totalEvalOps: " << totalEvalOps << ", iterated " << currentEvalOps << std::endl;
    currentEvalOps = totalEvalOps; // reach 100%
    getGame()->renderOps();

    if(Logger::log.isDebugEnabled())
    {
        Logger::log << "placeChoice: " << (placeChoice.valid ? "OK" : "NO") << " " << placeChoice.score << " (" << placeChoice.action << ") @ " << placeChoice.x << "," << placeChoice.y << std::endl;
        Logger::log << "removeChoice: " << (removeChoice.valid ? "OK" : "NO") << " " << removeChoice.score << " (" << removeChoice.action << ") @ " << removeChoice.x << "," << removeChoice.y << std::endl;
        Logger::log << "rotateChoice: " << (rotateChoice.valid ? "OK" : "NO") << " " << rotateChoice.score << " (" << rotateChoice.action << ") @ " << rotateChoice.x << "," << rotateChoice.y << std::endl;
    }


    if(placeChoice.valid && !removeChoice.valid && !rotateChoice.valid)
    {
        turn_choice = placeChoice;
    }
    else if(!placeChoice.valid && removeChoice.valid && !rotateChoice.valid)
    {
        turn_choice = removeChoice;
    }
    else if(!placeChoice.valid && !removeChoice.valid && rotateChoice.valid)
    {
        turn_choice = rotateChoice;
    }
    else
    {
        if(placeChoice.score > removeChoice.score)
        {
            turn_choice = placeChoice;
        }
        else if(placeChoice.score == removeChoice.score)
        {
            turn_choice = (rand() % 100 < 50) ? placeChoice : removeChoice;
        }
        else
        {
            turn_choice = removeChoice;
        }
        if(rotateChoice.score > turn_choice.score)
        {
            turn_choice = rotateChoice;
        }
        else if(rotateChoice.score == turn_choice.score)
        {
            if(rand() % 100 < 50)
                turn_choice = rotateChoice;
        }
    }
    Logger::log << turn_choice.score << " (" << turn_choice.action << ") @ " << turn_choice.x << "," << turn_choice.y << std::endl;

}
void AI::doComputationPlace(IATurnChoice *thisChoice, int threadId)
{
    if(!getGame()->getGrid()->isFull()) // grille pleine, pas d'ajout
    {
        int r_eval;
        int evalMax = EVAL_MIN;
        Grid grid(*getGame()->getGrid());
        Logger::log << "PLACE_TOKEN " << threadId << std::endl;
        for (int i = 0; i < grid.getWidth(); ++i)
        {

            int placePos = grid.getGravityProvider()->getFirstEmptyCell(i);
            if(placePos == -1)
            {
                //totalEvalOps -= totalEvalOpsByLevel[difficulty - 1];
                continue;
            }

            grid.setGridAt(i, placePos, getEntityIndex());
            r_eval = alphabeta(grid, difficulty - 1, nextEntityIndex, EVAL_MIN, EVAL_MAX, threadId);
            if(r_eval > evalMax || (r_eval == evalMax && rand() % 100 < 50))
            {
                evalMax = r_eval;
                thisChoice->set(TOKEN_PLACE, r_eval, i, placePos);
            }

            grid.clone(*getGame()->getGrid());
        }
    }
}
void AI::doComputationRemove(IATurnChoice *thisChoice, int threadId)
{
    if(getGame()->getGrid()->getCellsForPlayer(getEntityIndex()) > 0) // pas de jetons, pas de remove
    {
        int r_eval;
        int evalMax = EVAL_MIN;
        Grid grid(*getGame()->getGrid());
        Logger::log << "REMOVE_TOKEN " << threadId << std::endl;
        for (int i = 0; i < grid.getWidth(); ++i)
        {
            for (int j = 0; j < grid.getHeight(); ++j)
            {

                if(grid.getGridAt(i, j) != getEntityIndex())
                {
                    //totalEvalOps -= totalEvalOpsByLevel[difficulty - 1];
                    continue;
                }

                grid.forceSetGridAt(i, j, 0);
                grid.getGravityProvider()->doColumnGravity(i, NULL);

                r_eval = alphabeta(grid, difficulty - 1, nextEntityIndex, EVAL_MIN, EVAL_MAX, threadId);
                if(r_eval > evalMax || (r_eval == evalMax && rand() % 100 < 50))
                {
                    evalMax = r_eval;
                    thisChoice->set(TOKEN_REMOVE, r_eval, i, j);
                }

                grid.clone(*getGame()->getGrid());
            }
        }
    }
}
void AI::doComputationRotate(IATurnChoice *thisChoice, int threadId)
{
    if(getGame()->getGrid()->getFilledCells() >= getGame()->getGameSettings()->getAlignSize()) //
    {
        int r_eval;
        int evalMax = EVAL_MIN;
        Grid grid(*getGame()->getGrid());
        Logger::log << "ROTATE " << threadId << std::endl;
        for (int i = -1; i <= 1; i += 2)
        {
            grid.rotate(i);
            grid.getGravityProvider()->doGravity(NULL);

            r_eval = alphabeta(grid, difficulty - 1, nextEntityIndex, EVAL_MIN, EVAL_MAX, threadId);
            if(r_eval > evalMax || (r_eval == evalMax && rand() % 100 < 50))
            {
                evalMax = r_eval;
                thisChoice->set(ROTATE, r_eval, i, -1);
            }

            grid.clone(*getGame()->getGrid());
        }
    }
}

int AI::prune(Grid &grid, int &prof, int &nextEntityIndex, int &thisEntityIndex, int &alpha, int &beta, int &t_alpha, int &t_beta, const int &threadId)
{
    int value;
    if(thisEntityIndex != getEntityIndex()) // noeud MIN
    {
        value = alphabeta(grid, prof - 1, nextEntityIndex, alpha, Helpers::__min(beta, t_beta), threadId);
        t_beta = Helpers::__min(t_beta, value);
        if(alpha >= t_beta)
        {
            //Logger::log << "beta prune" << std::endl;
            return t_beta;
        }
    }
    else // noeud MAX
    {
        value = alphabeta(grid, prof - 1, nextEntityIndex, Helpers::__max(alpha, t_alpha), beta, threadId);
        t_alpha = Helpers::__max(t_alpha, value);
        if(t_alpha >= beta)
        {
            //Logger::log << "alpha prune" << std::endl;
            return t_alpha;
        }
    }
    return 0;
}

int AI::alphabeta(Grid parent_grid, int prof, int thisEntityIndex, int alpha, int beta, const int &threadId)
{
#ifdef _REENTRANT
    //g_mutex.lock();
#endif
    int r_eval = eval(parent_grid, prof, threadId);
#ifdef _REENTRANT
    //g_mutex.unlock();
#endif
    if(prof == 0 || this->winnerChecker->hasWinner())
        return r_eval;

    int nextEntityIndex = (thisEntityIndex % getGame()->getGameSettings()->getNumPlayers()) + 1;
    int value;
    int t_alpha = EVAL_MIN;
    int t_beta = EVAL_MAX;

    Grid grid(parent_grid);

    /***************************/
    /******* PLACE_TOKEN *******/
    /***************************/
    if(!grid.isFull()) // grille pleine, pas d'ajout
    {
        for (int i = 0; i < grid.getWidth(); ++i)
        {
            int placePos = grid.getGravityProvider()->getFirstEmptyCell(i);
            if(placePos == -1)
                continue;

            grid.setGridAt(i, placePos, thisEntityIndex);

            value = prune(grid, prof, nextEntityIndex, thisEntityIndex, alpha, beta, t_alpha, t_beta, threadId);
            if(value != 0)
                return value;


            grid.clone(parent_grid);
        }
    }

    /***************************/
    /******* REMOVE_TOKEN ******/
    /***************************/
    if(grid.getCellsForPlayer(thisEntityIndex) > 0) // pas de jetons, pas de remove
    {
        for (int i = 0; i < grid.getWidth(); ++i)
        {
            for (int j = 0; j < grid.getHeight(); ++j)
            {

                if(grid.getGridAt(i, j) != thisEntityIndex)
                    continue;

                grid.forceSetGridAt(i, j, 0);
                grid.getGravityProvider()->doColumnGravity(i, NULL);

                value = prune(grid, prof, nextEntityIndex, thisEntityIndex, alpha, beta, t_alpha, t_beta, threadId);
                if(value != 0)
                    return value;

                grid.clone(parent_grid);
            }
        }
    }

    /***************************/
    /********* ROTATE **********/
    /***************************/
    // ROTATE_CLOCKWISE = 1
    // ROTATE_COUNTERCLOCKWISE = -1
    if(grid.getFilledCells() >= getGame()->getGameSettings()->getAlignSize()) //
    {
        for (int i = -1; i <= 1; i += 2)
        {
            grid.rotate(i);
            grid.getGravityProvider()->doGravity(NULL);

            value = prune(grid, prof, nextEntityIndex, thisEntityIndex, alpha, beta, t_alpha, t_beta, threadId);
            if(value != 0)
                return value;

            grid.clone(parent_grid);
        }
    }
    return (thisEntityIndex == getEntityIndex()) ? t_alpha : t_beta;
}

int AI::eval(Grid &grid, const int &prof, const int &threadId)
{
    if(prof == 0)
    {
        currentEvalOps++;
        getGame()->renderOps();
    }
    WinnerChecker &localWinnerChecker = this->winnerChecker[threadId];
    localWinnerChecker.searchWinner(&grid, true);

    if(localWinnerChecker.hasWinner())
    {
        if(localWinnerChecker.hasDraw())
        {
            return 0;
        }
        return localWinnerChecker.isWinner(getEntityIndex() - 1) ? EVAL_MAX - prof : EVAL_MIN + prof;
    }
    int score = 0;
    int multiplier;
    for (int i = 0; i < getGame()->getGameSettings()->getNumPlayers(); ++i)
    {
        multiplier = (getEntityIndex() - 1 == i) ? 1 : -1;
        int algn = localWinnerChecker.getMaxAlignSize(i);
        if(algn <= 1)
            algn = 0;



        score += (localWinnerChecker.getNumWinAlignements(i) * 100 +
                  algn * 50 +
                  localWinnerChecker.getNumAlign(i) * 2/* +
                  grid.getCellsForPlayer(i+1)*/) * multiplier;
    }
    return score;
}



int AI::turn()
{
    Logger::log << "(" << getEntityIndex() << ")(" << difficulty << ") Entity-AI: turn" << std::endl;
    return 0;
}

void AI::init()
{
    nextEntityIndex = (getEntityIndex() % getGame()->getGameSettings()->getNumPlayers()) + 1;
    Logger::log << "(" << getEntityIndex() << ")(" << difficulty << ") Entity-AI: init, nextEntityIndex: " << nextEntityIndex << std::endl;

}

UpdateState AI::update(chtype)
{
    Logger::log << "(" << getEntityIndex() << ")(" << difficulty << ") Entity-AI: update" << std::endl;
    clock_t begin = std::clock();
    srand(static_cast<unsigned int>(time(NULL)));
    if(getGame()->getGrid()->isEmpty()) // premier tour ?
    {
        turn_choice.action = TOKEN_PLACE;
        turn_choice.x = (rand() % (getGame()->getGrid()->getWidth() - 4)) + 2;
        turn_choice.valid = true;
    }
    else
    {
        startAIComputation();
    }
    clock_t end = std::clock();
    double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
    Logger::log << "(" << getEntityIndex() << ")(" << difficulty << ") Entity-AI: computation end, took " << elapsed_secs << " sec" << std::endl;
    if(!turn_choice.valid)
    {
        Logger::log << "(" << getEntityIndex() << ")(" << difficulty << ") AI failed, not valid. retry.." << std::endl;
        return FAILURE;
    }
    if(!getGame()->onEntityTurnCompleted(turn_choice.action, turn_choice.x, turn_choice.y))
    {
        Logger::log << "(" << getEntityIndex() << ")(" << difficulty << ") AI fail turn, retrying" << std::endl;
        return FAILURE;
    }
    else if(this->adaptative)
    {
        if(turn_choice.score <= 0)
        {
            Logger::log << "(" << getEntityIndex() << ")(" << difficulty << ") Incrementing difficulty to ";
            setDifficulty(difficulty + 1);
            Logger::log << difficulty << std::endl;
        }
        else if(turn_choice.score > 10)
        {
            Logger::log << "(" << getEntityIndex() << ")(" << difficulty << ") Decrementing difficulty to ";
            setDifficulty(difficulty - 1);
            Logger::log << difficulty << std::endl;
        }
    }
    return SUCCESS;
}
void AI::render()
{

}

const char *AI::getId()
{
    return "Master Control Program";
}
int AI::getEntityType()
{
    return ENTITY_AI;
}
