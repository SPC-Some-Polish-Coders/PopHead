#ifndef POPHEAD_STATES_STATE_H
#define POPHEAD_STATES_STATE_H

#include <SFML/System.hpp>


namespace PopHead{

namespace Base { class GameData; }

namespace States{


class State
{
public:
    virtual void input() = 0;
    virtual void update(sf::Time delta) = 0;

    ///&World::Entity getRoot() const;
    bool getHide() const;
    bool getPause() const;

    void setHide(bool hide);
    void setPause(bool pause);

private:
    ///World::Entity mRoot;
    bool mHide;
    bool mPause;
};


}}

#endif // !POPHEAD_STATES_STATE_H
