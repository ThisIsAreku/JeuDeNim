#include "entities/AI.h"
#include "entities/EntityListener.h"

#include <ctime>
#include <cmath>
#include "Logger.h"

AI::AI(Game *game, int entityIndex, int level) : Entity(game, entityIndex)
{
    this->winnerChecker = new WinnerChecker(game, false);
    this->difficulty = level;
    //Logger::log << EVAL_MIN << "," << EVAL_MAX << std::endl;
}
AI::~AI()
{
    delete this->winnerChecker;
}

int AI::getOperationPercent() const
{
    return (currentEvalOps * 100) / totalEvalOps;
}

void AI::startAIComputation()
{
    using namespace std::placeholders;
    int r_eval;

    Grid grid(*getGame()->getGrid());

    // cols + delete + rotate
    int numPossibles = getGame()->getGrid()->getWidth()/* + getGame()->getGrid()->getTotalCells() + 2*/;
    totalEvalOps = numPossibles;
    /*for(int i = difficulty; i > 0; --i)
        totalEvalOps += pow(numPossibles, i);*/

    currentEvalOps = 0;
    Logger::log << "totalEvalOps: " << totalEvalOps << std::endl;

    cellChoiceC = 0;
    int cellFinalChoiceC = 0;
    IATurnChoice *cellChoice = new IATurnChoice[numPossibles];
    IATurnChoice *cellFinalChoice = new IATurnChoice[numPossibles];

    int evalMax = EVAL_MIN;
    getGame()->renderOps();

    for (int i = 0; i < grid.getWidth(); ++i)
    {
        currentEvalOps++;
        getGame()->renderOps();

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
    Logger::log << "totalEvalOps: " << totalEvalOps << ", iterated " << currentEvalOps << std::endl;

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
    return best;
}

int AI::eval(Grid &grid, const int &prof)
{
    this->winnerChecker->searchWinner(&grid, true);

    if(this->winnerChecker->hasWinner())
    {
        if(this->winnerChecker->hasDraw())
            return 0;
        return this->winnerChecker->isWinner(getEntityIndex() - 1) ? EVAL_MAX-prof : EVAL_MIN+prof;
    }
    int score = 0;
    int multiplier;
    for (int i = 0; i < getGame()->getGameSettings()->getNumPlayers(); ++i)
    {
        multiplier = (getEntityIndex() - 1 == i) ? 1 : -1;
        int algn = this->winnerChecker->getMaxAlignSize(i);
        if(algn <= 1)
            algn = 0;

        score += ((algn * 10) + this->winnerChecker->getNumAlign(i)) * multiplier;
    }
    return score;
}



int AI::turn()
{
    Logger::log << "(" << getEntityIndex() << ")(" << difficulty << ") Entity-AI: turn" << std::endl;
    clock_t begin = std::clock();
    srand(time(NULL));
    if(getGame()->getGrid()->getCellsForPlayer(getEntityIndex() - 1) < 1)
    {
        turn_choice.action = TOKEN_PLACE;
        turn_choice.x = rand() % getGame()->getGrid()->getWidth();
    }
    else
    {
        startAIComputation();
        //startAIComputation(getGame()->getGrid()->getWidth(), );
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

UpdateState AI::update(int)
{
    if(turn_choice.x == -1)
    {
        Logger::log << "(" << getEntityIndex() << ")(" << difficulty << ") AI failed" << std::endl;
        return FAILURE;
    }
    if(!getGame()->onEntityTurnCompleted(turn_choice.action, turn_choice.x, turn_choice.y))
    {
        Logger::log << "(" << getEntityIndex() << ")(" << difficulty << ") AI fail turn, retrying" << std::endl;
        turn();
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