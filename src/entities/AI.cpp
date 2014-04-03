#include "entities/AI.h"
#include "entities/EntityListener.h"


#include "Logger.h"

AI::AI(Game *game, int entityIndex) : Entity(game, entityIndex)
{
    tree = new NaryTree;
}
AI::~AI()
{
}

int AI::turn()
{

    return 0;
}

void AI::init()
{
}
UpdateState AI::update(int)
{

    return SUCCESS;
}
void AI::render()
{

}

const char *AI::getId()
{
    return "Master Control Program";
}
int AI::getEntityType()
{
    return ENTITY_AI;
}