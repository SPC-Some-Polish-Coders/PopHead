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
#include "ECS/Systems/staticCollisions.hpp"
#include "ECS/Systems/levers.hpp"
#include "ECS/Systems/gates.hpp"
#include "ECS/Systems/gunAttacks.hpp"
#include "ECS/Systems/gunPositioningAndTexture.hpp"
#include "ECS/Systems/velocityChangingAreas.hpp"
#include "ECS/Systems/meleeAttacks.hpp"
#include "ECS/Systems/meleePositioning.hpp"
#include "ECS/Systems/canUseWeapon.hpp"
#include "ECS/Systems/lifetime.hpp"
#include "ECS/Systems/animationSystem.hpp"
#include "ECS/Systems/particleSystem.hpp"
#include "ECS/Systems/pushingAreas.hpp"
#include "ECS/Systems/kinematicCollisions.hpp"
#include "ECS/Systems/velocityClear.hpp"
#include "ECS/Systems/audioSystem.hpp"
#include "ECS/Systems/zombieSystem.hpp"
#include "ECS/Systems/entrances.hpp"
#include "ECS/Systems/gameplayUI.hpp"

namespace ph {

Scene::Scene(MusicPlayer& musicPlayer, SoundPlayer& soundPlayer, AIManager& aiManager, Terminal& terminal, SceneManager& sceneManager, GUI& gui)
	:mCutSceneManager()
	,mSystemsQueue(mRegistry)
	,mPause(false)
{
	initiateSystemsQueue(musicPlayer, soundPlayer, aiManager, sceneManager, gui);
	terminal.setSceneRegistry(&mRegistry);
}

void Scene::handleEvent(const ph::Event& e)
{
}

void Scene::update(sf::Time dt)
{
 	if(mCutSceneManager.isCutSceneActive())
		mCutSceneManager.updateCutScene(dt);

	mSystemsQueue.update(dt.asSeconds());
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

void Scene::initiateSystemsQueue(MusicPlayer& musicPlayer, SoundPlayer& soundPlayer, AIManager& aiManager,
                                 SceneManager& sceneManager, GUI& gui)
{
	mSystemsQueue.appendSystem<system::RenderSystem>();
	mSystemsQueue.appendSystem<system::PatricleSystem>();
	mSystemsQueue.appendSystem<system::GameplayUI>(std::ref(gui));
	mSystemsQueue.appendSystem<system::PlayerMovementInput>();
	mSystemsQueue.appendSystem<system::ZombieSystem>(&aiManager);
	mSystemsQueue.appendSystem<system::KinematicCollisions>();
	mSystemsQueue.appendSystem<system::PlayerCameraMovement>();
	mSystemsQueue.appendSystem<system::PickupBullet>();
	mSystemsQueue.appendSystem<system::PickupMedkit>();
	mSystemsQueue.appendSystem<system::HostileCollisions>();
	mSystemsQueue.appendSystem<system::StaticCollisions>();
	mSystemsQueue.appendSystem<system::IsPlayerAlive>();
	mSystemsQueue.appendSystem<system::VelocityChangingAreas>();
	mSystemsQueue.appendSystem<system::PushingAreas>();
	mSystemsQueue.appendSystem<system::CanUseWeapon>();
	mSystemsQueue.appendSystem<system::GunPositioningAndTexture>();
	mSystemsQueue.appendSystem<system::GunAttacks>();
	mSystemsQueue.appendSystem<system::MeleeAttacks>();
	mSystemsQueue.appendSystem<system::MeleePositioning>();
	mSystemsQueue.appendSystem<system::DamageAndDeath>();
	mSystemsQueue.appendSystem<system::Levers>();
	mSystemsQueue.appendSystem<system::Movement>();
	mSystemsQueue.appendSystem<system::Gates>();
	mSystemsQueue.appendSystem<system::Lifetime>();
	mSystemsQueue.appendSystem<system::AnimationSystem>();
	mSystemsQueue.appendSystem<system::VelocityClear>();
	mSystemsQueue.appendSystem<system::EntityDestroying>();
	mSystemsQueue.appendSystem<system::Entrances>(std::ref(sceneManager));
	mSystemsQueue.appendSystem<system::AudioSystem>(std::ref(musicPlayer), std::ref(soundPlayer));
}

}
