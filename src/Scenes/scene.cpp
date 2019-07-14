#include "scene.hpp"
#include "gameData.hpp"
#include <SFML/Graphics.hpp>

namespace ph {

Scene::Scene(GameData* const gameData, const std::string& sceneSourceCodeFilePath)
	:mRoot(std::make_unique<Entity>(gameData, "root"))
	,mSceneParser(gameData, *mRoot.get(), sceneSourceCodeFilePath)
	,mGameData(gameData)
	,mHide(false)
	,mPause(false)
{
}

void Scene::input()
{
	mRoot->input();
}

void Scene::update(sf::Time delta)
{
	mRoot->update(delta);
}

}