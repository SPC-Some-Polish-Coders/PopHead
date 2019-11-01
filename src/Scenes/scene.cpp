#include "scene.hpp"
#include "cutScene.hpp"
#include "gameData.hpp"

#include "ECS/Systems/playerInput.hpp"
#include "ECS/Systems/movement.hpp"
#include "ECS/Systems/spritesSync.hpp"
#include "ECS/Systems/rendererSystem.hpp"

#include <SFML/Graphics.hpp>

namespace ph {

Scene::Scene(sf::RenderWindow& window)
	: mCutSceneManager()
	, mSystemsQueue(mRegistry)
	, mPause(false)
{
	initiateSystemsQueue(window);
}

void Scene::handleEvent(const ph::Event& e)
{
}

void Scene::update(sf::Time delta)
{
 	if(mCutSceneManager.isCutSceneActive())
		mCutSceneManager.updateCutScene(delta);
}

void Scene::setPlayerStatus(const PlayerStatus& status)
{
	//auto& player = getPlayer();
	//player.setHp(status.mHealthPoints);
	//player.setNumOfBullets(status.mNumOfBullets);
}

PlayerStatus Scene::getPlayerStatus() const
{
	/*auto& player = getPlayer();
	PlayerStatus status;
	status.mHealthPoints = player.getHp();
	status.mNumOfBullets = player.getNumOfBullets();
	return status;*/
	return PlayerStatus();
}

void Scene::initiateSystemsQueue(sf::RenderWindow& window)
{
	mSystemsQueue.appendSystem<system::PlayerInput>();
	mSystemsQueue.appendSystem<system::Movement>();
	mSystemsQueue.appendSystem<system::SpritesSync>();
	mSystemsQueue.appendSystem<system::Renderer>(std::ref(window));
}

}