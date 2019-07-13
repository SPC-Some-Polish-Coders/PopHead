#include "scene.hpp"
#include "gameData.hpp"
#include "EntityComponentSystem/entityType.hpp"
#include <SFML/Graphics.hpp>

namespace ph {

Scene::Scene(GameData* const gameData, const std::string& sceneSourceCodeFilePath)
	:mRoot(EntityType::none, gameData, "root")
	,mSceneParser(gameData, mRoot, sceneSourceCodeFilePath)
	,mGameData(gameData)
	,mHide(false)
	,mPause(false)
{
}

void Scene::input()
{
	mRoot.input();
}

void Scene::update(sf::Time delta)
{
	mRoot.update(delta);
}

}