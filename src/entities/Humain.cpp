#include "entities/Humain.h"


#include "Logger.h"

Humain::Humain(Game *game, int entityIndex) : super(game, entityIndex)
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
    Logger::log << "Entity-Humain: turn" << std::endl;
    if(!getGame()->getGrid()->isFull())
    {
        turnAction = TOKEN_PLACE;
        this->cursor->setCursorSize(1, getGame()->getGrid()->getHeight());
        this->cursor->setVisible(true);
    }
    else
    {
        turnAction = TOKEN_REMOVE;
        this->cursor->setCursorSize(1, 1);
        this->cursor->setVisible(true);
    }
    return 0;
}

void Humain::init()
{
    this->cursor->init();

}
UpdateState Humain::update(chtype ch)
{
    switch(ch)
    {
    case 'r':
        turnAction = ROTATE;
        getGame()->onEntityTurnCompleted(turnAction, ROTATE_CLOCKWISE, -1);
        break;
    case 't':
        turnAction = ROTATE;
        getGame()->onEntityTurnCompleted(turnAction, ROTATE_COUNTERCLOCKWISE, -1);
        break;
        /*case 'p':
            if(getGame()->getGrid()->isFull())
            {
                cdtOk = false;
                break;
            }
            turnAction = TOKEN_PLACE;
            this->cursor->setCursorSize(1, getGame()->getGrid()->getHeight());
            this->cursor->setVisible(true);
            break;*/
    case 'd':
        Logger::log << "Doing delete" << std::endl;
        if(getGame()->getGrid()->isEmpty())
        {
            return FAILURE;
        }
        turnAction = TOKEN_REMOVE;
        this->cursor->setCursorSize(1, 1);
        this->cursor->setVisible(true);
        break;
    default:
        UpdateState r = this->cursor->update(ch);
        if(r == REMOVE)
        {
            this->cursor->setVisible(false);
            return SUCCESS;
        }
        return r;
    }

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
