#pragma once

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

	void setPause(bool pause) { mPause = pause; }
	bool getPause() const { return mPause; }
	void setHide(bool hide) { mHide = hide; }
	bool getHide() const { return mHide; }
	
	Entity& getRoot() { return mRoot; }

protected:
    GameData* const mGameData;
    Entity mRoot;

private:
    bool mHide;
    bool mPause;
};

}
