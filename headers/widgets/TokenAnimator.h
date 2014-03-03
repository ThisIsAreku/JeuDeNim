#ifndef _TOKEN_ANIMATOR_H_
#define _TOKEN_ANIMATOR_H_

#include "BaseGrid.h"
#include "display/Renderable.h"

class TokenAnimator : public Renderable
{
    BaseGrid *_grid;
public:
    TokenAnimator(WindowManager *, int, BaseGrid *);
    void animateToken(int, int, int, int);

    void init();
    UpdateState update(chtype);
    void render();


};

#endif


