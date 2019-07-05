#pragma once

#include <SFML/System.hpp>
#include "World/Entity/entity.hpp"
#include "States/sceneParser.hpp"

namespace ph{

class GameData; 

class State
{
public:
    State(GameData* const gameData, const std::string& sceneSourceCodeFilePath);
    void input();
    void update(sf::Time delta);

	void setPause(bool pause) { mPause = pause; }
	bool getPause() const { return mPause; }
	void setHide(bool hide) { mHide = hide; }
	bool getHide() const { return mHide; }
	
	Entity& getRoot() { return mRoot; }

private:
	Entity mRoot;
	SceneParser mSceneParser;
	GameData* const mGameData;
    bool mHide;
    bool mPause;
};

}
