#include "entities/Random.h"
#include "entities/EntityListener.h"
#include <cstdlib>
#include <ctime>


#include "Logger.h"

Random::Random(BaseGame *game, int entityIndex) : Entity(game, entityIndex)
{
    srand(time(NULL));
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
        x = rand() % getBaseGame()->getBaseGrid()->getWidth();
        y = -1;
        return getBaseGame()->onEntityTurnCompleted(TOKEN_PLACE, x, y);

    case 5:
        x = rand() % getBaseGame()->getBaseGrid()->getWidth();
        y = rand() % getBaseGame()->getBaseGrid()->getHeight();
        return getBaseGame()->onEntityTurnCompleted(TOKEN_REMOVE, x, y);

    case 6:
    case 7:
        x = -1;
        y = -1;
        return getBaseGame()->onEntityTurnCompleted(ROTATE_CLOCKWISE, x, y);

    case 8:
    case 9:
        x = -1;
        y = -1;
        return getBaseGame()->onEntityTurnCompleted(ROTATE_COUNTERCLOCKWISE, x, y);
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
UpdateState Random::update(int)
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