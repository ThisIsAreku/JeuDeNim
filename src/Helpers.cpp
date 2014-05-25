#include "Helpers.hpp"

// helper
void Helpers::__switch_vars(int &a, int &b)
{
    int c = a;
    a = b;
    b = c;
}

int Helpers::__min(const int &a, const int &b)
{
    if(a < b)
        return a;
    return b;
}
int Helpers::__max(const int &a, const int &b)
{
    if(a > b)
        return a;
    return b;
}
