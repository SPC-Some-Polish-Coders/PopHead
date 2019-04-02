#ifndef POPHEAD_STATES_STATE_H
#define POPHEAD_STATES_STATE_H

#include <SFML/System.hpp>
#include "World/Entity/entity.hpp"

namespace PopHead{

namespace Base { class GameData; }

namespace States{


class State
{
public:
    State( Base::GameData* const gameData );
    virtual void input() = 0;
    virtual void update(sf::Time delta) = 0;

    ///&World::Entity getRoot() const;
    bool getHide() const;
    bool getPause() const;

    void setHide(bool hide);
    void setPause(bool pause);

protected:
    Base::GameData* const mGameData;

private:
    World::Entity::Entity mRoot;
    bool mHide;
    bool mPause;
};


}}

#endif // !POPHEAD_STATES_STATE_H
