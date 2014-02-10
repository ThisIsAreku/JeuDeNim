#include <iostream>

#include "WindowManager.h"
#include "Grille.h"
#include "Jeu.h"

Jeu::Jeu()
{
    this->manager = new WindowManager();
}
Jeu::~Jeu()
{
    //delete this->grille;
    delete this->manager;
}




/***********/
/* PUBLICS */
/***********/
void Jeu::afficheGrille()
{   
    this->manager->print(WIN_SCOREBOARD, 1, 1, "Hello");
    /*char str[80];
    getstr(str);
    WINDOW* win = getWindow(WIN_SCOREBOARD);
    mvwprintw(win, 0, 0, "You Entered: %s", str);
    wrefresh(win);
    int ch;
    while((ch = getch()) != KEY_F(2))
    {
    }*/
}