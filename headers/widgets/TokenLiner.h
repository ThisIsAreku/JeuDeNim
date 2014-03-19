#ifndef _TOKEN_LINER_H_
#define _TOKEN_LINER_H_

#include "display/Renderable.h"

#include "structs/TokenAlignement.h"

class TokenLiner : public Renderable
{
public:
    TokenLiner(WindowManager *, int);

    void displayAlignement(TokenAlignement align);

    void init();
    UpdateState update(chtype);
    void render();


};

#endif


