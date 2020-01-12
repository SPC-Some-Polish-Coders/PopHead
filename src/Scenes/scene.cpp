#include "scene.hpp"
#include "cutScene.hpp"
#include "gameData.hpp"

#include "ECS/Systems/playerMovementInput.hpp"
#include "ECS/Systems/movement.hpp"
#include "ECS/Systems/pushingMovement.hpp"
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
#include "ECS/Systems/lifetime.hpp"
#include "ECS/Systems/animationSystem.hpp"
#include "ECS/Systems/particleSystem.hpp"
#include "ECS/Systems/pushingAreas.hpp"
#include "ECS/Systems/hintAreas.hpp"
#include "ECS/Systems/kinematicCollisions.hpp"
#include "ECS/Systems/velocityClear.hpp"
#include "ECS/Systems/audioSystem.hpp"
#include "ECS/Systems/zombieSystem.hpp"
#include "ECS/Systems/entrances.hpp"
#include "ECS/Systems/gameplayUI.hpp"
#include "ECS/Systems/areasDebug.hpp"
#include "ECS/Systems/cars.hpp"
#include "ECS/Systems/cutscenesActivating.hpp"

#include "ECS/Components/charactersComponents.hpp"
#include "ECS/Components/physicsComponents.hpp"
#include "ECS/Components/itemComponents.hpp"

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
	mSystemsQueue.appendSystem<system::PlayerMovementInput>(std::ref(aiManager), std::ref(gui), this);
	mSystemsQueue.appendSystem<system::ZombieSystem>(&aiManager);
	mSystemsQueue.appendSystem<system::HostileCollisions>();
	mSystemsQueue.appendSystem<system::KinematicCollisions>();
	mSystemsQueue.appendSystem<system::Movement>();
	mSystemsQueue.appendSystem<system::PlayerCameraMovement>();
	mSystemsQueue.appendSystem<system::PickupItems>();
	mSystemsQueue.appendSystem<system::StaticCollisions>();
	mSystemsQueue.appendSystem<system::AreasDebug>();
	mSystemsQueue.appendSystem<system::IsPlayerAlive>();
	mSystemsQueue.appendSystem<system::VelocityChangingAreas>();
	mSystemsQueue.appendSystem<system::PushingAreas>();
	mSystemsQueue.appendSystem<system::HintAreas>(std::ref(gui));
	mSystemsQueue.appendSystem<system::GunPositioningAndTexture>();
	mSystemsQueue.appendSystem<system::GunAttacks>();
	mSystemsQueue.appendSystem<system::MeleeAttacks>();
	mSystemsQueue.appendSystem<system::DamageAndDeath>(std::ref(gui), std::ref(aiManager));
	mSystemsQueue.appendSystem<system::Levers>();
	mSystemsQueue.appendSystem<system::PushingMovement>();
	mSystemsQueue.appendSystem<system::Gates>();
	mSystemsQueue.appendSystem<system::Lifetime>();
	mSystemsQueue.appendSystem<system::AnimationSystem>();
	mSystemsQueue.appendSystem<system::VelocityClear>();
	mSystemsQueue.appendSystem<system::EntityDestroying>();
	mSystemsQueue.appendSystem<system::Entrances>(std::ref(sceneManager));
	mSystemsQueue.appendSystem<system::AudioSystem>(std::ref(musicPlayer), std::ref(soundPlayer));
	mSystemsQueue.appendSystem<system::Cars>();
	mSystemsQueue.appendSystem<system::CutScenesActivating>(std::ref(mCutSceneManager), std::ref(gui), std::ref(musicPlayer), std::ref(soundPlayer), std::ref(aiManager), std::ref(sceneManager));
}

void Scene::handleEvent(const ActionEvent& event)
{
	mSystemsQueue.handleEvents(event);
}

void Scene::update(sf::Time dt)
{
	if(mPause)
		return;

	const bool isCutsceneActive = mCutSceneManager.isCutSceneActive();
 	if(isCutsceneActive)
		mCutSceneManager.updateCutScene(dt);

	if(!isCutsceneActive || (isCutsceneActive && !mCutSceneManager.pausesSystems()))
		mSystemsQueue.update(dt.asSeconds());
}

void Scene::setPlayerStatus(const PlayerStatus& status)
{
	auto playerView = mRegistry.view<component::Bullets, component::Health, component::Player>();
	auto& [bullets, health] = playerView.get<component::Bullets, component::Health>(*playerView.begin());
	bullets.numOfPistolBullets = status.numOfPistolBullets;
	bullets.numOfShotgunBullets = status.numOfShotgunBullets;
	health.healthPoints = status.healthPoints;
}

PlayerStatus Scene::getPlayerStatus()
{
	auto playerView = mRegistry.view<component::Bullets, component::Health, component::Player>();
	for(auto player : playerView) {
		const auto [bullets, health] = playerView.get<component::Bullets, component::Health>(player);
		return PlayerStatus{health.healthPoints, bullets.numOfPistolBullets, bullets.numOfShotgunBullets};
	}
	return PlayerStatus{};
}

void Scene::setPlayerPosition(sf::Vector2f newPosition)
{
	auto playerView = mRegistry.view<component::Player, component::BodyRect>();
	auto& bodyRect = playerView.get<component::BodyRect>(*playerView.begin());
	bodyRect.rect.setPosition(newPosition);
}

entt::registry& Scene::getRegistry()
{
	return mRegistry;
}

}

