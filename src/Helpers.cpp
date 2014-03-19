#include "Helpers.h"

// helper
void Helpers::__switch_vars(int &a, int &b)
{
    int c = a;
    a = b;
    b = c;
}