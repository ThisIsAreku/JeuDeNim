#ifndef _TOKEN_ANIMATOR_H_
#define _TOKEN_ANIMATOR_H_

#include "BaseGrid.h"
#include "display/Renderable.h"

class TokenAnimator : public Renderable
{
    BaseGame *baseGame;
    double modifier;
public:
    TokenAnimator(WindowManager *, int, BaseGame *);
    void animateToken(int, int, int, int);

    void setModifier(double);

    void init();
    UpdateState update(chtype);
    void render();


};

#endif


