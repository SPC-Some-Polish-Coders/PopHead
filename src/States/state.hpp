#ifndef POPHEAD_STATES_STATE_H
#define POPHEAD_STATES_STATE_H

#include <SFML/System.hpp>
#include "World/Entity/entity.hpp"

namespace ph{

namespace Base { class GameData; }

class State
{
public:
    State(GameData* const gameData);
    virtual void input() = 0;
    virtual void update(sf::Time delta) = 0;

	bool getHide() const { return mHide; }
	bool getPause() const { return mPause; }

	void setHide(bool hide) { mHide = hide; }
	void setPause(bool pause) { mPause = pause; }

protected:
    GameData* const mGameData;
    Entity mRoot;

private:
    bool mHide;
    bool mPause;
};

}

#endif // !POPHEAD_STATES_STATE_H
