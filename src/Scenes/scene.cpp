#include "pch.hpp"
#include "scene.hpp"
#include "Utilities/threadPool.hpp"
#include "Terminal/terminal.hpp"

#include "ECS/Systems/playerMovementInput.hpp"
#include "ECS/Systems/movement.hpp"
#include "ECS/Systems/playerCameraMovement.hpp"
#include "ECS/Systems/entityDestroying.hpp"
#include "ECS/Systems/pickupSystem.hpp"
#include "ECS/Systems/renderSystem.hpp"
#include "ECS/Systems/damageAndDeath.hpp"
#include "ECS/Systems/hostileCollisions.hpp"
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
#include "ECS/Systems/audioSystem.hpp"
#include "ECS/Systems/zombieSystem.hpp"
#include "ECS/Systems/entrances.hpp"
#include "ECS/Systems/gameplayUI.hpp"
#include "ECS/Systems/areasDebug.hpp"
#include "ECS/Systems/debugCamera.hpp"
#include "ECS/Systems/weather.hpp"
#include "ECS/Systems/slowZombieSystem.hpp"
#include "ECS/Systems/entitiesDebugger.hpp"
#include "ECS/Systems/indoorOutdoorBlending.hpp"
#include "ECS/Systems/pressurePlates.hpp"
#include "ECS/Systems/puzzles.hpp"
#include "ECS/Systems/spikes.hpp"
#include "ECS/Systems/save.hpp"
#include "ECS/Systems/teleport.hpp"
#include "ECS/Systems/puzzleBoulders.hpp"

#include "ECS/Components/charactersComponents.hpp"
#include "ECS/Components/physicsComponents.hpp"
#include "ECS/Components/itemComponents.hpp"

namespace ph {

Scene::Scene(AIManager& aiManager, SceneManager& sceneManager, Texture& tilesetTexture,
             ThreadPool& threadPool, EntitiesTemplateStorage& entitiesTemplateStorage, sf::Window* window)
	:mSystemsQueue(mRegistry, threadPool)
{
	// should be at the start
	mSystemsQueue.appendSystem<system::RenderSystem>(std::ref(tilesetTexture));

	// must be before Movement
	mSystemsQueue.appendSystem<system::PlayerMovementInput>(std::ref(aiManager), this);
	mSystemsQueue.appendSystem<system::ZombieSystem>(&aiManager, std::ref(threadPool));
	mSystemsQueue.appendSystem<system::VelocityChangingAreas>();
	mSystemsQueue.appendSystem<system::PushingAreas>();

	mSystemsQueue.appendSystem<system::Movement>(); // physics

	mSystemsQueue.appendSystem<system::GunPositioningAndTexture>(); // must be after Movement and before GunAttacks
	mSystemsQueue.appendSystem<system::GunAttacks>();
	mSystemsQueue.appendSystem<system::MeleeAttacks>();

	mSystemsQueue.appendSystem<system::HostileCollisions>(); // must be after Movement and before KinematicCollisions
	mSystemsQueue.appendSystem<system::SlowZombieSystem>(); // must be after HostileCollisions and before ZombieSystem (in next iteration)

	mSystemsQueue.appendSystem<system::DamageAndDeath>(std::ref(aiManager)); // must be after GunAttacks, MeleeAttacks and HostileCollisions
	
	mSystemsQueue.appendSystem<system::PatricleSystem>(); // must be after DamageAndDeath

	mSystemsQueue.appendSystem<system::KinematicCollisions>(); // physics

	mSystemsQueue.appendSystemWithLastOrder<system::GameplayUI>(); // must be after DamageAndDeath

	mSystemsQueue.appendSystem<system::Gates>(); // must be after Levers and before StaticCollisions

	mSystemsQueue.appendSystem<system::StaticCollisions>(); // physics

	mSystemsQueue.appendSystemWithLastOrder<system::AnimationSystem>(); // must be after Levers and DamageAndDeath

	// should be after StaticCollisions
	mSystemsQueue.appendSystem<system::PlayerCameraMovement>();
	mSystemsQueue.appendSystem<system::AreasDebug>();
	mSystemsQueue.appendSystem<system::AudioSystem>();

	// must be after StaticCollisions
	mSystemsQueue.appendSystemWithLastOrder<system::PickupItems>();
	mSystemsQueue.appendSystem<system::HintAreas>();
	mSystemsQueue.appendSystem<system::Entrances>(std::ref(sceneManager));

	// must be after GunAttacks and before EntityDestroying
	mSystemsQueue.appendSystem<system::Lifetime>();

	// not specified yet
	mSystemsQueue.appendSystem<system::DebugCamera>();
	mSystemsQueue.appendSystem<system::Weather>();
	mSystemsQueue.appendSystem<system::EntitiesDebugger>(window);
	mSystemsQueue.appendSystem<system::IndoorOutdoorBlending>();
	mSystemsQueue.appendSystem<system::PressurePlates>();
	mSystemsQueue.appendSystem<system::Puzzles>(std::ref(entitiesTemplateStorage));
	mSystemsQueue.appendSystem<system::Spikes>();
	mSystemsQueue.appendSystem<system::Save>();
	mSystemsQueue.appendSystem<system::Teleport>();
	mSystemsQueue.appendSystem<system::PuzzleBoulders>();

	mSystemsQueue.appendSystem<system::Levers>(); // must be after Puzzles

	// must be at the end
	mSystemsQueue.appendSystem<system::EntityDestroying>();
}

void Scene::handleEvent(sf::Event e)
{
	mSystemsQueue.handleEvents(e);
}

void Scene::update(float dt)
{
	mSystemsQueue.update(dt);
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

void Scene::setPlayerPosition(Vec2 newPos)
{
	auto playerView = mRegistry.view<component::Player, component::BodyRect>();
	auto& bodyRect = playerView.get<component::BodyRect>(*playerView.begin());
	bodyRect.pos = newPos;
}

entt::registry& Scene::getRegistry()
{
	return mRegistry;
}

}

