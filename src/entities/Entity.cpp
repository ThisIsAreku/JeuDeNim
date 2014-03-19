#include "entities/Entity.h"


#include "Logger.h"
Entity::Entity(BaseGame *game, int entityIndex)
{
    this->game = game;
    this->entityIndex = entityIndex;
}
Entity::~Entity()
{

}
BaseGame *Entity::getBaseGame()
{
    return this->game;
}
int Entity::getEntityIndex()
{
    return this->entityIndex;
}