#ifndef _TOKEN_LINER_H_
#define _TOKEN_LINER_H_

#include "display/Renderable.hpp"

struct TokenAlignement;

class TokenLiner : public Renderable
{
    typedef Renderable super;
public:
    TokenLiner(WindowManager *, int);

    void displayAlignement(TokenAlignement align);

    void init();
    UpdateState update(chtype);
    void render();


};

#endif


