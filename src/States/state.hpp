#pragma once

#include <SFML/System.hpp>
#include "World/Entity/entity.hpp"
#include "States/sceneParser.hpp"

namespace ph{

class GameData; 

class State
{
public:
    State(GameData* const gameData);
    void input();
    void update(sf::Time delta);

	void setPause(bool pause) { mPause = pause; }
	bool getPause() const { return mPause; }
	void setHide(bool hide) { mHide = hide; }
	bool getHide() const { return mHide; }
	
	Entity& getRoot() { return mRoot; }

private:
	void windowMinimalizeAndMaximalizeShortcut() const;

protected:
	Entity mRoot;
	GameData* const mGameData;
	SceneParser mSceneParser;

private:
    bool mHide;
    bool mPause;
};

}
