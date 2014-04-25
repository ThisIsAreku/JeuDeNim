#ifndef _AI_H_
#define _AI_H_

#include <limits>   // std::numeric_limits

#include "entities/Entity.h"
#include "entities/EntityListener.h"
#include "WinnerChecker.h"


#include "Grid.h"

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

    int cellChoiceC;

    long totalEvalOps;
    long currentEvalOps;

    bool doRemove = false;
    bool doRotate = true;

    WinnerChecker *winnerChecker;

    void startAIComputation();

    int alphabeta(Grid, int, int, int, int);

    int eval(Grid &, const int &);

public:
    AI(Game *, int, int);
    virtual ~AI();

    int turn();

    void init();
    UpdateState update(int);
    void render();

    const char *getId();
    int getEntityType();

    int getOperationPercent() const;

};

#endif
