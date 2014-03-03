#ifndef _CHECKER_H_
#define _CHECKER_H_

#include "BaseGrid.h"

class Checker
{
    BaseGrid *_grid;

public:
    Checker(BaseGrid *);
    void update();
};

#endif