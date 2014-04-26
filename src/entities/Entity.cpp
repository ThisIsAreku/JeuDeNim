#include "entities/Entity.h"


#include "Logger.h"
Entity::Entity(Game *game, int entityIndex)
{
    Logger::log << "Init new Entity with index " << entityIndex << std::endl;
    this->game = game;
    this->entityIndex = entityIndex;
}
Entity::~Entity()
{

}
Game *Entity::getGame() const
{
    return this->game;
}
int Entity::getEntityIndex() const
{
    return this->entityIndex;
}
