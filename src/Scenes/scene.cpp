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
#include "ECS/Systems/collisionDebug.hpp"

#include "ECS/Components/charactersComponents.hpp"
#include "ECS/Components/physicsComponents.hpp"

namespace ph {

Scene::Scene(MusicPlayer& musicPlayer, SoundPlayer& soundPlayer, AIManager& aiManager, Terminal& terminal,
             SceneManager& sceneManager, GUI& gui, Texture& tilesetTexture)
	:mCutSceneManager()
	,mSystemsQueue(mRegistry)
	,mPause(false)
{
	terminal.setSceneRegistry(&mRegistry);

	mSystemsQueue.appendSystem<system::RenderSystem>(std::ref(tilesetTexture));
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
	mSystemsQueue.appendSystem<system::CollisionDebug>();
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

void Scene::handleEvent(const ActionEvent& event)
{
	mSystemsQueue.handleEvents(event);
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

void Scene::setPlayerPosition(sf::Vector2f newPosition)
{
	auto playerView = mRegistry.view<component::Player, component::BodyRect>();
	auto& bodyRect = playerView.get<component::BodyRect>(*playerView.begin());
	bodyRect.rect.left = newPosition.x;
	bodyRect.rect.top = newPosition.y;
}

entt::registry& Scene::getRegistry()
{
	return mRegistry;
}

}
