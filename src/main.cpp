#include <iostream>
#include "Game.h"


#include "Logger.h"
int main(int argc, char **argv)
{

    bool doDebug = false;
    for(int i = 1; i < argc; i++)
        if(strcmp(argv[i], "-debug") == 0)
            doDebug = true;

    Logger::log.setDebug(doDebug);
    Game game;
    game.start();

}