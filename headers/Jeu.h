#ifndef _JEU_H_
#define _JEU_H_

#include "Grille.h"
#include "WindowManager.h"

class Jeu
{
    Grille *grille;
    WindowManager* manager;

public:
    Jeu();
    ~Jeu();
    void afficheGrille();

};

#endif
