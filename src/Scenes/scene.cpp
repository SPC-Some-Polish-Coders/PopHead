#include "scene.hpp"
#include "cutScene.hpp"
#include "gameData.hpp"
#include "ECS/Systems/playerInput.hpp"
#include "ECS/Systems/movement.hpp"
#include "ECS/Systems/playerCameraMovement.hpp"
#include "ECS/Systems/entityDestroying.hpp"
#include "ECS/Systems/pickupSystem.hpp"
#include "ECS/Systems/renderSystem.hpp"
#include "ECS/Systems/damageAndDeath.hpp"
#include "ECS/Systems/hostileCollisions.hpp"
#include "ECS/Systems/isPlayerAlive.hpp"
#include "ECS/Systems/isObjectInArea.hpp"
#include "ECS/Systems/staticCollisions.hpp"
#include "ECS/Systems/levers.hpp"
#include "ECS/Systems/pendingGunAttacks.hpp"
#include "ECS/Systems/gunPositioningAndTexture.hpp"
#include "ECS/Systems/velocityChangingAreas.hpp"
#include "ECS/Systems/pendingMeleeAttacks.hpp"
#include "ECS/Systems/meleePositioning.hpp"
#include "ECS/Systems/canUseWeapon.hpp"
#include "ECS/Systems/lifetime.hpp"
#include "ECS/Systems/animationSystem.hpp"
#include "ECS/Systems/particleSystem.hpp"
#include "ECS/Systems/kinematicCollisions.hpp"
#include "ECS/Systems/velocityClear.hpp"
#include "ECS/Systems/audioSystem.hpp"
#include "ECS/Systems/zombieSystem.hpp"
#include "ECS/Systems/entrances.hpp"

namespace ph {

Scene::Scene(sf::Window& window, MusicPlayer& musicPlayer, SoundPlayer& soundPlayer, AIManager& aiManager, Terminal& terminal, SceneManager& sceneManager)
	:mCutSceneManager()
	,mSystemsQueue(mRegistry)
	,mPause(false)
{
	initiateSystemsQueue(window, musicPlayer, soundPlayer, aiManager, sceneManager);
	terminal.setSceneRegistry(&mRegistry);
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

void Scene::initiateSystemsQueue(sf::Window& window, MusicPlayer& musicPlayer, SoundPlayer& soundPlayer, AIManager& aiManager, SceneManager& sceneManager)
{
	mSystemsQueue.appendSystem<system::RenderSystem>(std::ref(window));
	mSystemsQueue.appendSystem<system::PatricleSystem>();
	mSystemsQueue.appendSystem<system::PlayerMovementInput>();
	mSystemsQueue.appendSystem<system::ZombieSystem>(&aiManager);
	mSystemsQueue.appendSystem<system::KinematicCollisions>();
	mSystemsQueue.appendSystem<system::Movement>();
	mSystemsQueue.appendSystem<system::PlayerCameraMovement>();
	mSystemsQueue.appendSystem<system::PickupBullet>();
	mSystemsQueue.appendSystem<system::PickupMedkit>();
	mSystemsQueue.appendSystem<system::HostileCollisions>();
	mSystemsQueue.appendSystem<system::StaticCollisions>();
	mSystemsQueue.appendSystem<system::IsPlayerAlive>();
	mSystemsQueue.appendSystem<system::IsObjectInArea>();
	mSystemsQueue.appendSystem<system::VelocityChangingAreas>();
	mSystemsQueue.appendSystem<system::CanUseGun>();
	mSystemsQueue.appendSystem<system::CanUseMelee>();
	mSystemsQueue.appendSystem<system::GunPositioningAndTexture>();
	mSystemsQueue.appendSystem<system::PendingGunAttacks>();
	mSystemsQueue.appendSystem<system::PendingMeleeAttacks>();
	mSystemsQueue.appendSystem<system::MeleePositioning>();
	mSystemsQueue.appendSystem<system::DamageAndDeath>();
	mSystemsQueue.appendSystem<system::Levers>();
	mSystemsQueue.appendSystem<system::Lifetime>();
	mSystemsQueue.appendSystem<system::AnimationSystem>();
	mSystemsQueue.appendSystem<system::VelocityClear>();
	mSystemsQueue.appendSystem<system::EntityDestroying>();
	mSystemsQueue.appendSystem<system::Entrances>(std::ref(sceneManager));
	mSystemsQueue.appendSystem<system::AudioSystem>(std::ref(musicPlayer), std::ref(soundPlayer));
}

}
