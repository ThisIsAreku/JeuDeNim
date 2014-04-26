#include <iostream>
#include "Game.h"
#include <cstdlib>


#include "Logger.h"
int main(int argc, char **argv)
{

    bool doDebug = false;
    bool doRndTest = false;
    for(int i = 1; i < argc; i++)
    {
        if(strcmp(argv[i], "-debug") == 0)
            doDebug = true;
        if(strcmp(argv[i], "-rndtest") == 0)
            doRndTest = true;
    }

    Logger::log.setDebug(doDebug);
    Game game;
    game.setRandomTesting(doRndTest);
    game.start();

}
