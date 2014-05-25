#ifndef _TOKEN_ANIMATOR_H_
#define _TOKEN_ANIMATOR_H_

#include "display/Renderable.hpp"

class Game;

class TokenAnimator : public Renderable
{
    typedef Renderable super;
    Game *game;
    double modifier;
public:
    TokenAnimator(WindowManager *, int, Game *);
    void animateToken(int, int, int, int);

    void setModifier(double);

    void init();
    UpdateState update(chtype);
    void render();


};

#endif


