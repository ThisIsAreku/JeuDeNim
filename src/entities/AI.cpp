#include "entities/AI.h"

#include <ctime>
#include <cmath>
#include "Logger.h"

AI::AI(Game *game, int entityIndex, int level, bool adaptative) : Entity(game, entityIndex)
{
    this->winnerChecker = new WinnerChecker(game, false);
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
    int r_eval;

    Grid grid(*getGame()->getGrid());

    // cols + delete + rotate
    numPossibles = getGame()->getGrid()->getWidth() + getGame()->getGrid()->getTotalCells() + 2;
    totalEvalOps = static_cast<long>(pow(numPossibles, difficulty));
    for(int i = 0; i < difficulty; ++i)
        totalEvalOpsByLevel[i] = static_cast<long>(pow(numPossibles, i));

    turn_choice.valid = false;

    currentEvalOps = 0;
    Logger::log << "numPossibles: " << numPossibles << std::endl;
    Logger::log << "totalEvalOps: " << totalEvalOps << std::endl;

    int cellChoiceC = 0;
    //int cellFinalChoiceC = 0;
    IATurnChoice *cellChoice = new IATurnChoice[numPossibles];
    //IATurnChoice *cellFinalChoice = new IATurnChoice[numPossibles];

    int evalMax = EVAL_MIN;
    getGame()->renderOps();

    /****************************/
    /******* PLACE_TOKEN *******/
    /**************************/
    if(!grid.isFull()) // grille pleine, pas d'ajout
    {
        Logger::log << "PLACE_TOKEN" << std::endl;
        for (int i = 0; i < grid.getWidth(); ++i)
        {

            int placePos = grid.getGravityProvider()->getFirstEmptyCell(i);
            if(placePos == -1)
            {
                //totalEvalOps -= totalEvalOpsByLevel[difficulty - 1];
                continue;
            }

            grid.setGridAt(i, placePos, getEntityIndex());
            r_eval = alphabeta(grid, difficulty - 1, nextEntityIndex, EVAL_MIN, EVAL_MAX);
            if(r_eval > evalMax)
            {
                cellChoiceC = 0;
                evalMax = r_eval;
            }
            if(r_eval == evalMax)
            {
                cellChoice[cellChoiceC++].set(TOKEN_PLACE, r_eval, i, placePos);
            }

            grid.clone(*getGame()->getGrid());
        }
    }

    /****************************/
    /******* REMOVE_TOKEN *******/
    /**************************/
    if(grid.getCellsForPlayer(getEntityIndex()) > 0) // pas de jetons, pas de remove
    {
        Logger::log << "REMOVE_TOKEN" << std::endl;
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

                r_eval = alphabeta(grid, difficulty - 1, nextEntityIndex, EVAL_MIN, EVAL_MAX);
                if(r_eval > evalMax)
                {
                    cellChoiceC = 0;
                    evalMax = r_eval;
                }
                if(r_eval == evalMax)
                {
                    cellChoice[cellChoiceC++].set(TOKEN_REMOVE, r_eval, i, j);
                }


                grid.clone(*getGame()->getGrid());
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
        Logger::log << "ROTATE" << std::endl;
        for (int i = -1; i <= 1; i += 2)
        {
            grid.rotate(i);
            grid.getGravityProvider()->doGravity(NULL);

            r_eval = alphabeta(grid, difficulty - 1, nextEntityIndex, EVAL_MIN, EVAL_MAX);
            if(r_eval > evalMax)
            {
                cellChoiceC = 0;
                evalMax = r_eval;
            }
            if(r_eval == evalMax)
            {
                cellChoice[cellChoiceC++].set(ROTATE, r_eval, i, -1);
            }

            grid.clone(*getGame()->getGrid());
        }
    }



    Logger::log << "totalEvalOps: " << totalEvalOps << ", iterated " << currentEvalOps << std::endl;
    currentEvalOps = totalEvalOps; // reach 100%
    getGame()->renderOps();


    if(Logger::log.isDebugEnabled())
    {
        Logger::log << "(" << getEntityIndex() << ")(" << difficulty << ") --------------- all turn calculated ------------" << std::endl;

        // partie aléatoire

        for (int i = 0; i < cellChoiceC; ++i) // <= car resultIndex = max-1
        {
            if(!cellChoice[i].valid)
                continue;
            Logger::log << "Index: " << i << ", ";
            Logger::log << "score: " << cellChoice[i].score;
            Logger::log << "\twith " << cellChoice[i].action << " @ " << cellChoice[i].x << "," << cellChoice[i].y;
            /*if(cellChoice[i].score == evalMax) // si le coup est idéal
            {
                Logger::log << " <--- ";
                cellFinalChoice[cellFinalChoiceC++] = cellChoice[i];
            }*/
            Logger::log << std::endl;
        }
        Logger::log << "(" << getEntityIndex() << ")(" << difficulty << ") ------------------------------------------------" << std::endl;
    }

    Logger::log << "(" << getEntityIndex() << ")(" << difficulty << ") ";
    if(cellChoiceC > 1) // si plusieurs coup idéaux
    {
        turn_choice = cellChoice[rand() % cellChoiceC];
        Logger::log << "Random: ";
    }
    else   // sinon, une seule possibilité
    {
        turn_choice = cellChoice[0];
        Logger::log << "First : ";
    }
    Logger::log << turn_choice.score << " (" << turn_choice.action << ") @ " << turn_choice.x << "," << turn_choice.y << std::endl;

    delete [] cellChoice;
    //delete [] cellFinalChoice;
}

int AI::prune(Grid &grid, int &prof, int &nextEntityIndex, int &thisEntityIndex, int &alpha, int &beta, int &t_alpha, int &t_beta)
{
    int value;
    if(thisEntityIndex != getEntityIndex()) // noeud MIN
    {
        value = alphabeta(grid, prof - 1, nextEntityIndex, alpha, Helpers::__min(beta, t_beta));
        t_beta = Helpers::__min(t_beta, value);
        if(alpha >= t_beta)
        {
            //Logger::log << "beta prune" << std::endl;
            return t_beta;
        }
    }
    else // noeud MAX
    {
        value = alphabeta(grid, prof - 1, nextEntityIndex, Helpers::__max(alpha, t_alpha), beta);
        t_alpha = Helpers::__max(t_alpha, value);
        if(t_alpha >= beta)
        {
            //Logger::log << "alpha prune" << std::endl;
            return t_alpha;
        }
    }
    return 0;
}

int AI::alphabeta(Grid parent_grid, int prof, int thisEntityIndex, int alpha, int beta)
{
    int r_eval = eval(parent_grid, prof);
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

            value = prune(grid, prof, nextEntityIndex, thisEntityIndex, alpha, beta, t_alpha, t_beta);
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

                value = prune(grid, prof, nextEntityIndex, thisEntityIndex, alpha, beta, t_alpha, t_beta);
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

            value = prune(grid, prof, nextEntityIndex, thisEntityIndex, alpha, beta, t_alpha, t_beta);
            if(value != 0)
                return value;

            grid.clone(parent_grid);
        }
    }
    return (thisEntityIndex == getEntityIndex()) ? t_alpha : t_beta;
}

int AI::eval(Grid &grid, const int &prof)
{
    if(prof == 0)
    {
        currentEvalOps++;
        getGame()->renderOps();
    }
    this->winnerChecker->searchWinner(&grid, true);

    if(this->winnerChecker->hasWinner())
    {
        if(this->winnerChecker->hasDraw())
            return 0;
        return this->winnerChecker->isWinner(getEntityIndex() - 1) ? EVAL_MAX - prof : EVAL_MIN + prof;
    }
    int score = 0;
    int multiplier;
    for (int i = 0; i < getGame()->getGameSettings()->getNumPlayers(); ++i)
    {
        multiplier = (getEntityIndex() - 1 == i) ? 1 : -1;
        int algn = this->winnerChecker->getMaxAlignSize(i);
        if(algn <= 1)
            algn = 0;



        score += (this->winnerChecker->getNumWinAlignements(i) * 100 +
                  algn * 50 +
                  this->winnerChecker->getNumAlign(i) * 2/* +
                  grid.getCellsForPlayer(i)*/) * multiplier;
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
