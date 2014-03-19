#include "entities/Humain.h"
#include "entities/EntityListener.h"


#include "Logger.h"

Humain::Humain(Game *game, int entityIndex) : Entity(game, entityIndex)
{

    this->cursor = new CellCursor(game->getWindowManager(), WIN_GAME_GRID, game->getGrid(), entityIndex + 4);
    this->cursor->setCellSelectedListener(std::bind((&Humain::onCursorSelect), this, std::placeholders::_1, std::placeholders::_2));
    this->cursor->setVisible(false);
}
Humain::~Humain()
{

    if(this->cursor != NULL)
        delete this->cursor;
}

int Humain::turn()
{
    Logger::log << "Entity-Humain: turn (nothing to do)" << std::endl;
    return 0;
}

void Humain::init()
{
    this->cursor->init();

}
UpdateState Humain::update(int ch)
{
    if(this->cursor->isVisible())
    {
        UpdateState r = this->cursor->update(ch);
        if(r == REMOVE)
        {
            this->cursor->setVisible(false);
            return SUCCESS;
        }
        return r;
    }

    bool cdtOk = true;
    do
    {
        cdtOk = true;
        switch(ch)
        {
        case 'r':
            turnAction = ROTATE_CLOCKWISE;
            getGame()->onEntityTurnCompleted(turnAction, -1, -1);
            break;
        case 't':
            turnAction = ROTATE_COUNTERCLOCKWISE;
            getGame()->onEntityTurnCompleted(turnAction, -1, -1);
            break;
        case 'p':
            if(getGame()->getGrid()->isFull())
            {
                cdtOk = false;
                break;
            }
            turnAction = TOKEN_PLACE;
            this->cursor->setCursorSize(1, getGame()->getGrid()->getHeight());
            this->cursor->setVisible(true);
            break;
        case 'd':
            if(getGame()->getGrid()->isEmpty())
            {
                cdtOk = false;
                break;
            }
            turnAction = TOKEN_REMOVE;
            this->cursor->setCursorSize(1, 1);
            this->cursor->setVisible(true);
            break;
        }
    }
    while(!cdtOk);

    return SUCCESS;
}
void Humain::render()
{
    if(this->cursor != NULL)
        return this->cursor->render();

}

const char *Humain::getId()
{
    return "HUMAIN";
}

bool Humain::onCursorSelect(int x, int y)
{
    return getGame()->onEntityTurnCompleted(turnAction, x, y);
}
int Humain::getEntityType()
{
    return ENTITY_HUMAIN;
}