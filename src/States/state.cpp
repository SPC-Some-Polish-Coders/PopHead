#include "States/state.hpp"

#include "World/Entity/entityType.hpp"
#include "gameData.hpp"
#include <SFML/Graphics.hpp>

namespace ph {

State::State(GameData* const gameData)
	:mRoot(EntityType::none, gameData, "root")
	,mGameData(gameData)
	,mSceneParser(gameData, mRoot, "scene.xml")
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