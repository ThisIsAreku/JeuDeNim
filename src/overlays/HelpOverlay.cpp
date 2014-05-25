#include "overlays/HelpOverlay.hpp"

#include "display/Window.hpp"

HelpOverlay::HelpOverlay(WindowManager *manager) : super(manager)
{
}
HelpOverlay::~HelpOverlay()
{
}

void HelpOverlay::init()
{}

UpdateState HelpOverlay::update(chtype)
{}

void HelpOverlay::render()
{
    Window *win = getWindow();
    if(win == NULL)
        return;

    win->clear();
    win->printAt(0, 0, "Donc là c'est l'aide...");

    int i = 2;
    win->printAt(0, i++, "Touches d'actions");
    win->printAt(0, i++, "- P: Placer un jeton");
    win->printAt(0, i++, "- D: Supprimer un jeton");
    win->printAt(0, i++, "- R: Rotation antihoraire");
    win->printAt(0, i++, "- T: Rotation horaire");

    i = 2;
    win->printAt(34, i++, "Déplacer le plateau");
    win->printAt(34, i++, "- O: Haut");
    win->printAt(34, i++, "- L: Bas");
    win->printAt(34, i++, "- K: Gauche");
    win->printAt(34, i++, "- M: Droite");
    i++;
    win->printAt(0, i, "Appuyez sur F6 pour sauvegarder, F7 pour charger");


    win->printAt(0, -1, "Appuyez sur [H] pour fermer l'aide");
}