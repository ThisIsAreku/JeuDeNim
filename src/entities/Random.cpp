#include "entities/Random.h"
#include <cstdlib>
#include <ctime>


#include "Logger.h"

Random::Random(Game *game, int entityIndex) : super(game, entityIndex)
{
    srand(static_cast<unsigned int>(time(NULL)));
}
Random::~Random()
{
}

bool Random::doChoice()
{
    int choice = rand() % 10;
    Logger::log << "Entity-Random " << getEntityIndex() << ": try choice : " << choice << std::endl;
    int x, y;
    switch(choice)
    {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
        x = rand() % getGame()->getGrid()->getWidth();
        y = -1;
        return getGame()->onEntityTurnCompleted(TOKEN_PLACE, x, y);

    case 5:
        x = rand() % getGame()->getGrid()->getWidth();
        y = rand() % getGame()->getGrid()->getHeight();
        return getGame()->onEntityTurnCompleted(TOKEN_REMOVE, x, y);

    case 6:
    case 7:
        x = ROTATE_CLOCKWISE;
        y = -1;
        return getGame()->onEntityTurnCompleted(ROTATE, x, y);

    case 8:
    case 9:
        x = ROTATE_COUNTERCLOCKWISE;
        y = -1;
        return getGame()->onEntityTurnCompleted(ROTATE, x, y);
    default:
        // empty
        break;
    }
    return false;
}

int Random::turn()
{

    return 0;
}

void Random::init()
{
}
UpdateState Random::update(chtype)
{
    while(!doChoice());

    return SUCCESS;
}
void Random::render()
{

}

const char *Random::getId()
{
    return "Stupid Monkey";
}
int Random::getEntityType()
{
    return ENTITY_DUMBASS;
}
