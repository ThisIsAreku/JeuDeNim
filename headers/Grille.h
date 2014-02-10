#ifndef _GRILLE_H_
#define _GRILLE_H_

class Grille
{
    int width;
    int height;

    int **matrice;

public:
    Grille(int width, int height);
    ~Grille();

};

#endif
