#pragma once

#include "EntityComponentSystem/entity.hpp"
#include "sceneParser.hpp"
#include <SFML/System.hpp>
#include <memory>

namespace ph{

class GameData; 

class Scene
{
public:
    Scene(GameData* const gameData, const std::string& sceneSourceCodeFilePath);
    void input();
    void update(sf::Time delta);

	void setPause(bool pause) { mPause = pause; }
	bool getPause() const { return mPause; }
	void setHide(bool hide) { mHide = hide; }
	bool getHide() const { return mHide; }
	
	Entity& getRoot() { return *mRoot.get(); }

private:
	std::unique_ptr<Entity> mRoot;
	SceneParser mSceneParser;
	GameData* const mGameData;
    bool mHide;
    bool mPause;
};

}
