#ifndef _AI_H_
#define _AI_H_

#include <limits>   // std::numeric_limits

#include "entities/Entity.h"
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
    int difficulty;
    bool adaptative;
    int nextEntityIndex;

    IATurnChoice turn_choice;
    static const int EVAL_MAX = 1000000;
    static const int EVAL_MIN = -1000000;

    int cellChoiceC;

    long totalEvalOps;
    long currentEvalOps;

    WinnerChecker *winnerChecker;

    void startAIComputation();

    int alphabeta(Grid, int, int, int, int);

    int eval(Grid &, const int &);

public:
    AI(Game *, int, int, bool);
    virtual ~AI();

    int turn();

    void init();
    UpdateState update(chtype);
    void render();

    int getDifficulty();
    bool isAdaptative();

    const char *getId();
    int getEntityType();

    double getOperationPercent() const;

};

#endif
