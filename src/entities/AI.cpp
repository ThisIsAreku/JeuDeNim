#include "entities/AI.h"
#include "entities/EntityListener.h"

#include <ctime>
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

void AI::startAIComputation()
{
    using namespace std::placeholders;
    int r_eval;

    Grid grid(*getGame()->getGrid());

    // cols + delete + rotate
    int numPossibles = getGame()->getGrid()->getWidth() + getGame()->getGrid()->getTotalCells() + 2;

    int cellChoiceC = -1;
    int cellFinalChoiceC = 0;
    IATurnChoice *cellChoice = new IATurnChoice[numPossibles];
    IATurnChoice *cellFinalChoice = new IATurnChoice[numPossibles];

    int evalMax = EVAL_MIN;

    int x = -1;
    int y = -1;

    IAComputationResultCode result = computeTokenPlace(grid, x, y, getEntityIndex());
    while(result != RESULT_STOP)
    {
        cellChoiceC += 1;
        cellChoice[cellChoiceC].set(TOKEN_PLACE, 0, x, y);
        if(result == RESULT_CONTINUE)
        {
            r_eval = OtherTurn(grid, difficulty, nextEntityIndex);
            cellChoice[cellChoiceC].score = r_eval;
            evalMax = Helpers::__max(r_eval, evalMax);
            grid.clone(*getGame()->getGrid());
        }
        else
        {
            cellChoice[cellChoiceC].valid = false; // coup impossible
        }
        result = computeTokenPlace(grid, x, y, getEntityIndex());
    }


    x = 0;
    y = 0;

    result = computeTokenRemove(grid, x, y, getEntityIndex());
    while(result != RESULT_STOP)
    {
        cellChoiceC += 1;
        cellChoice[cellChoiceC].set(TOKEN_REMOVE, 0, x, y);
        if(result == RESULT_CONTINUE)
        {
            r_eval = OtherTurn(grid, difficulty, nextEntityIndex);
            cellChoice[cellChoiceC].score = r_eval; // on minimise ce coup
            evalMax = Helpers::__max(r_eval, evalMax);
            grid.clone(*getGame()->getGrid());
        }
        else
        {
            cellChoice[cellChoiceC].valid = false; // coup impossible
        }
        result = computeTokenRemove(grid, x, y, getEntityIndex());
    }

    Logger::log << "(" << getEntityIndex() << ")(" << difficulty << ") --------------- all turn calculated ------------" << std::endl;

    // partie aléatoire
    for (int i = 0; i <= cellChoiceC; ++i) // <= car resultIndex = max-1
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

int AI::OtherTurn(Grid parent_grid, int prof, int otherEntityIndex)
{
    //MIN(AIturn)
    int r_eval = eval(parent_grid, prof);
    if(prof == 0 || this->winnerChecker->hasWinner())
        return r_eval;

    int evalMin = EVAL_MAX;
    int nextEntityIndex = (otherEntityIndex % getGame()->getGameSettings()->getNumPlayers()) + 1;

    Grid grid(parent_grid);

    int x = -1;
    int y = -1;

    IAComputationResultCode result = computeTokenPlace(grid, x, y, getEntityIndex());
    while(result != RESULT_STOP)
    {
        if(result == RESULT_CONTINUE)
        {
            if(nextEntityIndex == getEntityIndex())
                r_eval = AITurn(grid, prof - 1);
            else
                r_eval = OtherTurn(grid, prof - 1, nextEntityIndex);

            evalMin = Helpers::__min(evalMin, r_eval);
            grid.clone(parent_grid);
        }
        result = computeTokenPlace(grid, x, y, getEntityIndex());
    }


    x = 0;
    y = 0;

    result = computeTokenRemove(grid, x, y, getEntityIndex());
    while(result != RESULT_STOP)
    {
        if(result == RESULT_CONTINUE)
        {
            if(nextEntityIndex == getEntityIndex())
                r_eval = AITurn(grid, prof - 1);
            else
                r_eval = OtherTurn(grid, prof - 1, nextEntityIndex);

            evalMin = Helpers::__min(evalMin, r_eval);
            grid.clone(parent_grid);
        }
        result = computeTokenRemove(grid, x, y, getEntityIndex());
    }

    return evalMin;
}

int AI::AITurn(Grid parent_grid, int prof)
{
    //MAX
    int r_eval = eval(parent_grid, prof);
    if(prof == 0 || this->winnerChecker->hasWinner())
        return r_eval;

    int evalMax = EVAL_MIN;

    Grid grid(parent_grid);

    int x = -1;
    int y = -1;

    IAComputationResultCode result = computeTokenPlace(grid, x, y, getEntityIndex());
    while(result != RESULT_STOP)
    {
        if(result == RESULT_CONTINUE)
        {
            evalMax = Helpers::__max(evalMax, OtherTurn(grid, prof - 1, nextEntityIndex));
            grid.clone(parent_grid);
        }
        result = computeTokenPlace(grid, x, y, getEntityIndex());
    }


    x = 0;
    y = 0;

    result = computeTokenRemove(grid, x, y, getEntityIndex());
    while(result != RESULT_STOP)
    {
        if(result == RESULT_CONTINUE)
        {
            evalMax = Helpers::__max(evalMax, OtherTurn(grid, prof - 1, nextEntityIndex));
            grid.clone(parent_grid);
        }
        result = computeTokenRemove(grid, x, y, getEntityIndex());
    }

    return evalMax;
}

int AI::eval(const Grid &grid, const int &prof) const
{
    this->winnerChecker->searchWinner(&grid, true);

    if(this->winnerChecker->hasWinner())
    {
        if(this->winnerChecker->hasDraw())
        {
            return 0;
        }
        else if(!this->winnerChecker->isWinner(getEntityIndex() - 1))
        {
            return EVAL_MIN + grid.getFilledCells();
        }
        else
        {
            return EVAL_MAX - grid.getFilledCells();
        }
    }
    int score = 0;
    int pscore;
    int multiplier;
    for (int i = 0; i < getGame()->getGameSettings()->getNumPlayers(); ++i)
    {
        multiplier = (i == getEntityIndex() - 1) ? 1 : -1;

        pscore = this->winnerChecker->getMaxAlignSize(i);
        if(pscore <= 1)
            pscore = 0;

        score += ((pscore * 50) + grid.getCellsForPlayer(i)) * multiplier;
    }
    return score;
}


IAComputationResultCode AI::computeTokenPlace(Grid &grid, int &x, int &y, const int &entityIndex)
{
    ++x;
    if(x >= grid.getWidth())
        return RESULT_STOP;

    int placePos = grid.getGravityProvider()->getFirstEmptyCell(x);
    if(placePos == -1)
        return RESULT_FAILURE;

    grid.setGridAt(x, placePos, entityIndex);
    return RESULT_CONTINUE;

}
IAComputationResultCode AI::computeTokenRemove(Grid &grid, int &x, int &y, const int &entityIndex)
{
    if(++x >= grid.getWidth()){
        x = 0;
        if(++y >= grid.getHeight()){
            return RESULT_STOP;
        }
    }

    if(grid.getGridAt(x, y) <= 0)
        return RESULT_FAILURE;

    grid.forceSetGridAt(x, y, 0);
    grid.getGravityProvider()->doColumnGravity(x, NULL);
    return RESULT_CONTINUE;
}
IAComputationResultCode AI::computeRotate(Grid &grid, int &x, int &y, const int &entityIndex)
{
    if(x >= 2)
        return RESULT_STOP;
    return RESULT_STOP; //goto fail
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