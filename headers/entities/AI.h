#ifndef _AI_H_
#define _AI_H_

#include <limits>   // std::numeric_limits

#include "entities/Entity.h"
#include "WinnerChecker.h"


#include "Grid.h"

struct IATurnChoice
{
    EntityTurnAction action = TOKEN_PLACE;
    int score, x, y;
    bool valid;
    IATurnChoice() : action(TOKEN_PLACE), score(0), x(0), y(0), valid(false) {};
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

    int numPossibles;

    long *totalEvalOpsByLevel;
    long totalEvalOps;
    long currentEvalOps;

    WinnerChecker *winnerChecker;

    void startAIComputation();

    int alphabeta(Grid, int, int, int, int);
    int prune(Grid &, int &, int &, int &, int &, int &, int &, int &);

    int eval(Grid &, const int &);

public:
    AI(Game *, int, int, bool);
    virtual ~AI();

    int turn();

    void init();
    UpdateState update(chtype);
    void render();

    void setDifficulty(int);
    int getDifficulty();
    bool isAdaptative();

    const char *getId();
    int getEntityType();

    float getOperationPercent() const;

};

#endif
