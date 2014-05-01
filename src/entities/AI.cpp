#include "entities/AI.h"

#include <ctime>
#include <cmath>
#include "Logger.h"

AI::AI(Game *game, int entityIndex, int level) : Entity(game, entityIndex)
{
    this->winnerChecker = new WinnerChecker(game, false);
    if(level == 0)
    {
        this->adaptative = true;
        level = 2;
    }
    this->difficulty = level;
    Logger::log << "(" << entityIndex << ") Tree power ACTIVATE!" << std::endl;
}
AI::~AI()
{
    delete this->winnerChecker;
}

int AI::getDifficulty()
{
    return this->difficulty;
}
bool AI::isAdaptative()
{
    return this->adaptative;
}

double AI::getOperationPercent() const
{
    return (currentEvalOps * 100) / static_cast<double>(totalEvalOps);
}

void AI::startAIComputation()
{
    int r_eval;

    Grid grid(*getGame()->getGrid());

    // cols + delete + rotate
    int numPossibles = getGame()->getGrid()->getWidth() + getGame()->getGrid()->getTotalCells()/* + 2*/;
    totalEvalOps = static_cast<long>(pow(numPossibles, difficulty));

    turn_choice.valid = false;

    currentEvalOps = 0;
    Logger::log << "totalEvalOps: " << totalEvalOps << std::endl;

    cellChoiceC = 0;
    int cellFinalChoiceC = 0;
    IATurnChoice *cellChoice = new IATurnChoice[numPossibles];
    IATurnChoice *cellFinalChoice = new IATurnChoice[numPossibles];

    int evalMax = EVAL_MIN;
    getGame()->renderOps();

    /****************************/
    /******* PLACE_TOKEN *******/
    /**************************/

    for (int i = 0; i < grid.getWidth(); ++i)
    {

        int placePos = grid.getGravityProvider()->getFirstEmptyCell(i);
        if(placePos == -1)
            continue;

        grid.setGridAt(i, placePos, getEntityIndex());
        r_eval = alphabeta(grid, difficulty, nextEntityIndex, EVAL_MIN, EVAL_MAX);
        if(r_eval > evalMax)
        {
            cellChoiceC = 0;
            evalMax = r_eval;
        }
        if(r_eval == evalMax)
            cellChoice[cellChoiceC++].set(TOKEN_PLACE, r_eval, i, placePos);

        grid.clone(*getGame()->getGrid());
    }

    /****************************/
    /******* REMOVE_TOKEN *******/
    /**************************/

    for (int i = 0; i < grid.getWidth(); ++i)
    {
        for (int j = 0; j < grid.getHeight(); ++j)
        {

            if(grid.getGridAt(i, j) != getEntityIndex())
                continue;

            grid.forceSetGridAt(i, j, 0);
            grid.getGravityProvider()->doColumnGravity(i, NULL);

            r_eval = alphabeta(grid, difficulty, nextEntityIndex, EVAL_MIN, EVAL_MAX);
            if(r_eval > evalMax)
            {
                cellChoiceC = 0;
                evalMax = r_eval;
            }
            if(r_eval == evalMax)
                cellChoice[cellChoiceC++].set(TOKEN_REMOVE, r_eval, i, j);


            grid.clone(*getGame()->getGrid());
        }
    }

    /***************************/
    /********* ROTATE **********/
    /***************************/
    // ROTATE_CLOCKWISE = 1
    // ROTATE_COUNTERCLOCKWISE = -1

    for (int i = -1; i <= 1; i += 2)
    {
        grid.rotate(i);
        grid.getGravityProvider()->doGravity(NULL);

        r_eval = alphabeta(grid, difficulty, nextEntityIndex, EVAL_MIN, EVAL_MAX);
        if(r_eval > evalMax)
        {
            cellChoiceC = 0;
            evalMax = r_eval;
        }
        if(r_eval == evalMax)
            cellChoice[cellChoiceC++].set(ROTATE, r_eval, i, -1);

        grid.clone(*getGame()->getGrid());
    }



    Logger::log << "totalEvalOps: " << totalEvalOps << ", iterated " << currentEvalOps << std::endl;
    currentEvalOps = totalEvalOps; // reach 100%
    getGame()->renderOps();


    Logger::log << "(" << getEntityIndex() << ")(" << difficulty << ") --------------- all turn calculated ------------" << std::endl;

    // partie aléatoire
    for (int i = 0; i < cellChoiceC; ++i) // <= car resultIndex = max-1
    {
        if(!cellChoice[i].valid)
            continue;
        Logger::log << "Index: " << i << ", ";
        Logger::log << "score: " << cellChoice[i].score;
        Logger::log << "\twith " << cellChoice[i].action << " @ " << cellChoice[i].x << "," << cellChoice[i].y;
        if(cellChoice[i].score == evalMax) // si le coup est idéal
        {
            Logger::log << " <--- ";
            cellFinalChoice[cellFinalChoiceC++] = cellChoice[i];
        }
        Logger::log << std::endl;
    }
    Logger::log << "(" << getEntityIndex() << ")(" << difficulty << ") ------------------------------------------------" << std::endl;

    Logger::log << "(" << getEntityIndex() << ")(" << difficulty << ") ";
    if(cellFinalChoiceC > 1) // si plusieurs coup idéaux
    {
        turn_choice = cellFinalChoice[rand() % cellFinalChoiceC];
        Logger::log << "Random: ";
    }
    else   // sinon, une seule possibilité
    {
        turn_choice = cellFinalChoice[0];
        Logger::log << "First : ";
    }
    Logger::log << turn_choice.score << " (" << turn_choice.action << ") @ " << turn_choice.x << "," << turn_choice.y << std::endl;

    delete [] cellChoice;
    delete [] cellFinalChoice;
}

int AI::alphabeta(Grid parent_grid, int prof, int thisEntityIndex, int alpha, int beta)
{
    int r_eval = eval(parent_grid, prof);
    if(prof == 0 || this->winnerChecker->hasWinner())
        return r_eval;

    int nextEntityIndex = (thisEntityIndex % getGame()->getGameSettings()->getNumPlayers()) + 1;
    int best = (thisEntityIndex == getEntityIndex()) ? EVAL_MIN : EVAL_MAX;
    int value;
    Grid grid(parent_grid);

    /***************************/
    /******* PLACE_TOKEN *******/
    /***************************/

    for (int i = 0; i < grid.getWidth(); ++i)
    {
        int placePos = grid.getGravityProvider()->getFirstEmptyCell(i);
        if(placePos == -1)
            continue;

        grid.setGridAt(i, placePos, thisEntityIndex);

        value = alphabeta(grid, prof - 1, nextEntityIndex, alpha, beta);

        if(thisEntityIndex == getEntityIndex()) // noeud MAX
        {
            best = Helpers::__max(best, value);
            if(best >= beta) // coupure beta
                return best;

            alpha = Helpers::__max(alpha, best);
        }
        else // noeud min
        {
            best = Helpers::__min(best, value);
            if(alpha >= best) // coupure alpha
                return best;

            beta = Helpers::__min(beta, best);
        }


        grid.clone(parent_grid);
    }

    /***************************/
    /******* REMOVE_TOKEN ******/
    /***************************/

    for (int i = 0; i < grid.getWidth(); ++i)
    {
        for (int j = 0; j < grid.getHeight(); ++j)
        {

            if(grid.getGridAt(i, j) != thisEntityIndex)
                continue;

            grid.forceSetGridAt(i, j, 0);
            grid.getGravityProvider()->doColumnGravity(i, NULL);

            value = alphabeta(grid, prof - 1, nextEntityIndex, alpha, beta);

            if(thisEntityIndex == getEntityIndex()) // noeud MAX (self)
            {
                best = Helpers::__max(best, value);
                if(best >= beta) // coupure beta
                    return best;

                alpha = Helpers::__max(alpha, best);
            }
            else // noeud min
            {
                best = Helpers::__min(best, value);
                if(alpha >= best) // coupure alpha
                    return best;

                beta = Helpers::__min(beta, best);
            }


            grid.clone(parent_grid);
        }
    }

    /***************************/
    /********* ROTATE **********/
    /***************************/
    // ROTATE_CLOCKWISE = 1
    // ROTATE_COUNTERCLOCKWISE = -1

    for (int i = -1; i <= 1; i += 2)
    {
        grid.rotate(i);
        grid.getGravityProvider()->doGravity(NULL);

        value = alphabeta(grid, prof - 1, nextEntityIndex, alpha, beta);

        if(thisEntityIndex == getEntityIndex()) // noeud MAX
        {
            best = Helpers::__max(best, value);
            if(best >= beta) // coupure beta
                return best;

            alpha = Helpers::__max(alpha, best);
        }
        else // noeud min
        {
            best = Helpers::__min(best, value);
            if(alpha >= best) // coupure alpha
                return best;

            beta = Helpers::__min(beta, best);
        }

        grid.clone(parent_grid);
    }
    return best;
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
                  this->winnerChecker->getNumAlign(i) * 2 +
                  grid.getCellsForPlayer(i)) * multiplier;
    }
    return score;
}



int AI::turn()
{
    Logger::log << "(" << getEntityIndex() << ")(" << difficulty << ") Entity-AI: turn" << std::endl;
    clock_t begin = std::clock();
    srand(static_cast<unsigned int>(time(NULL)));
    if(getGame()->getGrid()->getCellsForPlayer(getEntityIndex() - 1) < 1)
    {
        turn_choice.action = TOKEN_PLACE;
        turn_choice.x = rand() % getGame()->getGrid()->getWidth();
        turn_choice.valid = true;
    }
    else
    {
        startAIComputation();
    }
    clock_t end = std::clock();
    double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
    Logger::log << "(" << getEntityIndex() << ")(" << difficulty << ") Entity-AI: computation end, took " << elapsed_secs << " sec" << std::endl;
    return 0;
}

void AI::init()
{
    nextEntityIndex = (getEntityIndex() % getGame()->getGameSettings()->getNumPlayers()) + 1;
    Logger::log << "(" << getEntityIndex() << ")(" << difficulty << ") Entity-AI: init, nextEntityIndex: " << nextEntityIndex << std::endl;

}

UpdateState AI::update(chtype)
{
    if(!turn_choice.valid)
    {
        Logger::log << "(" << getEntityIndex() << ")(" << difficulty << ") AI failed, not valid. retry.." << std::endl;
        turn();
        return FAILURE;
    }
    if(!getGame()->onEntityTurnCompleted(turn_choice.action, turn_choice.x, turn_choice.y))
    {
        Logger::log << "(" << getEntityIndex() << ")(" << difficulty << ") AI fail turn, retrying" << std::endl;
        turn();
    }
    else if(this->adaptative)
    {
        if(turn_choice.score <= 0)
        {
            Logger::log << "(" << getEntityIndex() << ")(" << difficulty << ") Incrementing difficulty to ";

            this->difficulty++;
            if(this->difficulty > 15)
                this->difficulty = 15;
            Logger::log << difficulty << std::endl;
        }
        else
        {
            Logger::log << "(" << getEntityIndex() << ")(" << difficulty << ") Decrementing difficulty to ";
            this->difficulty--;
            if(this->difficulty <= 0)
                this->difficulty = 1;
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
