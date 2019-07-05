#include "States/state.hpp"

#include "EntityComponentSystem/entityType.hpp"
#include "gameData.hpp"
#include <SFML/Graphics.hpp>

namespace ph {

State::State(GameData* const gameData, const std::string& sceneSourceCodeFilePath)
	:mRoot(EntityType::none, gameData, "root")
	,mGameData(gameData)
	,mSceneParser(gameData, mRoot, sceneSourceCodeFilePath)
	,mHide(false)
	,mPause(false)
{
}

void State::input()
{
	mRoot.input();
}

void State::update(sf::Time delta)
{
	mRoot.update(delta);
}

}