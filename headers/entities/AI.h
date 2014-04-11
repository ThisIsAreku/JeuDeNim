#ifndef _AI_H_
#define _AI_H_

#include <limits>   // std::numeric_limits

#include "entities/Entity.h"
#include "entities/EntityListener.h"
#include "WinnerChecker.h"


#include "Grid.h"

enum IAComputationResultCode
{
    RESULT_CONTINUE,
    RESULT_STOP,
    RESULT_FAILURE
};
struct IAComputationResult
{
    IAComputationResultCode resultCode;
    int resultIndex;
    IAComputationResult(IAComputationResultCode _resultCode, int _resultIndex) : resultCode(_resultCode), resultIndex(_resultIndex) {};
};
struct IATurnChoice
{
    EntityTurnAction action = TOKEN_PLACE;
    int score = 0;
    int x = 0;
    int y = 0;
    bool valid = false;
    void set(EntityTurnAction _action, int _score, int _x, int _y)
    {
        action = _action;
        score = _score;
        x = _x;
        y = _y;
        valid = true;
    }
};

class AI : public Entity
{
    int difficulty = 1;
    int nextEntityIndex;

    IATurnChoice turn_choice;
    int EVAL_MAX = 10000000;
    int EVAL_MIN = -10000000;

    WinnerChecker *winnerChecker;

    void startAIComputation();


    int OtherTurn(Grid, int, int); //min
    int AITurn(Grid, int);        //max

    int eval(const Grid &, const int &) const;

    IAComputationResultCode computeTokenPlace(Grid &grid, int &x, int &y, const int &entityIndex);
    IAComputationResultCode computeTokenRemove(Grid &grid, int &x, int &y, const int &entityIndex);
    IAComputationResultCode computeRotate(Grid &grid, int &x, int &y, const int &entityIndex);

public:
    AI(Game *, int, int);
    virtual ~AI();

    int turn();

    void init();
    UpdateState update(int);
    void render();

    const char *getId();
    int getEntityType();

};

#endif
