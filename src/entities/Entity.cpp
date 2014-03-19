#include "entities/Entity.h"


#include "Logger.h"
Entity::Entity(Game *game, int entityIndex)
{
    this->game = game;
    this->entityIndex = entityIndex;
}
Entity::~Entity()
{

}
Game *Entity::getGame()
{
    return this->game;
}
int Entity::getEntityIndex()
{
    return this->entityIndex;
}