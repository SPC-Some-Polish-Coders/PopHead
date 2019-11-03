#include "scene.hpp"
#include "cutScene.hpp"
#include "gameData.hpp"
#include "ECS/Systems/dyingCharacters.hpp"
#include "ECS/Systems/entityDestroying.hpp"
#include "ECS/Systems/movement.hpp"
#include "ECS/Systems/pickupSystem.hpp"
#include "ECS/Systems/playerInput.hpp"
#include "ECS/Systems/renderSystem.hpp"

namespace ph {

Scene::Scene(sf::Window& window)
	:mCutSceneManager()
	,mSystemsQueue(mRegistry)
	,mPause(false)
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

	mSystemsQueue.update(delta.asSeconds());
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

entt::registry& Scene::getRegistry()
{
	return mRegistry;
}

void Scene::initiateSystemsQueue(sf::Window& window)
{
	mSystemsQueue.appendSystem<system::PlayerInput>();
	mSystemsQueue.appendSystem<system::Movement>();
	mSystemsQueue.appendSystem<system::PickupBullet>();
	mSystemsQueue.appendSystem<system::PickupMedkit>();
	mSystemsQueue.appendSystem<system::DyingCharacters>();
	mSystemsQueue.appendSystem<system::EntityDestroying>();
	mSystemsQueue.appendSystem<system::RenderSystem>(std::ref(window));
}

}
