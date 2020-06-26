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
#include "ECS/Systems/lifetimeSystem.hpp"
#include "ECS/Systems/animationSystem.hpp"
#include "ECS/Systems/particleSystem.hpp"
#include "ECS/Systems/pushingAreas.hpp"
#include "ECS/Systems/hintAreas.hpp"
#include "ECS/Systems/kinematicCollisions.hpp"
#include "ECS/Systems/audioSystem.hpp"
#include "ECS/Systems/zombieSystem.hpp"
#include "ECS/Systems/gameplayUI.hpp"
#include "ECS/Systems/debugVisualization.hpp"
#include "ECS/Systems/debugCameraSystem.hpp"
#include "ECS/Systems/weatherSystem.hpp"
#include "ECS/Systems/slowZombieSystem.hpp"
#include "ECS/Systems/entitiesDebugger.hpp"
#include "ECS/Systems/indoorOutdoorBlending.hpp"
#include "ECS/Systems/pressurePlates.hpp"
#include "ECS/Systems/puzzles.hpp"
#include "ECS/Systems/spikesSystem.hpp"
#include "ECS/Systems/savePoints.hpp"
#include "ECS/Systems/teleport.hpp"
#include "ECS/Systems/puzzleBoulders.hpp"
#include "ECS/Systems/platforms.hpp"
#include "ECS/Systems/pits.hpp"
#include "ECS/Systems/simRegion.hpp"

#include "ECS/Components/charactersComponents.hpp"
#include "ECS/Components/physicsComponents.hpp"
#include "ECS/Components/itemComponents.hpp"

#include "ECS/entityUtil.hpp"

namespace ph {

Scene::Scene(AIManager& aiManager, SceneManager& sceneManager, Texture& tilesetTexture,
             ThreadPool& threadPool, EntitiesTemplateStorage& entitiesTemplateStorage, sf::Window* window)
	:mSystemsQueue(mRegistry, threadPool)
{
	using namespace system;

	// must be at the start
	mSystemsQueue.appendSystem<SimRegion>();

	// should be between SpatialPartitioning and every other system
	mSystemsQueue.appendSystem<RenderSystem>(std::ref(tilesetTexture));

	// must be before Movement
	mSystemsQueue.appendSystem<PlayerMovementInput>(std::ref(aiManager), this);
	mSystemsQueue.appendSystem<ZombieSystem>(&aiManager, std::ref(threadPool));
	mSystemsQueue.appendSystem<VelocityChangingAreas>();
	mSystemsQueue.appendSystem<PushingAreas>();

	mSystemsQueue.appendSystem<Movement>(); // physics

	mSystemsQueue.appendSystem<GunPositioningAndTexture>(); // must be after Movement and before GunAttacks
	mSystemsQueue.appendSystem<GunAttacks>();
	mSystemsQueue.appendSystem<MeleeAttacks>();

	mSystemsQueue.appendSystem<HostileCollisions>(); // must be after Movement and before KinematicCollisions
	mSystemsQueue.appendSystem<SlowZombieSystem>(); // must be after HostileCollisions and before ZombieSystem (in next iteration)

	mSystemsQueue.appendSystem<DamageAndDeath>(std::ref(aiManager)); // must be after GunAttacks, MeleeAttacks and HostileCollisions
	
	mSystemsQueue.appendSystem<PatricleSystem>(); // must be after DamageAndDeath

	mSystemsQueue.appendSystem<KinematicCollisions>(); // physics

	mSystemsQueue.appendSystemWithLastOrder<GameplayUI>(); // must be after DamageAndDeath

	mSystemsQueue.appendSystem<Gates>(); // must be after Levers and before StaticCollisions

	mSystemsQueue.appendSystem<StaticCollisions>(); // physics

	mSystemsQueue.appendSystemWithLastOrder<AnimationSystem>(); // must be after Levers and DamageAndDeath

	// should be after StaticCollisions
	mSystemsQueue.appendSystem<PlayerCameraMovement>();
	mSystemsQueue.appendSystem<DebugVisualization>();
	mSystemsQueue.appendSystem<AudioSystem>();

	// must be after StaticCollisions
	mSystemsQueue.appendSystemWithLastOrder<PickupItems>();
	mSystemsQueue.appendSystem<HintAreas>();

	// must be after GunAttacks and before EntityDestroying
	mSystemsQueue.appendSystem<LifetimeSystem>();

	// not specified yet
	mSystemsQueue.appendSystem<DebugCameraSystem>();
	mSystemsQueue.appendSystem<WeatherSystem>();
	mSystemsQueue.appendSystem<EntitiesDebugger>(window);
	mSystemsQueue.appendSystem<IndoorOutdoorBlending>();
	mSystemsQueue.appendSystem<PressurePlates>();
	mSystemsQueue.appendSystem<Puzzles>(std::ref(entitiesTemplateStorage));
	mSystemsQueue.appendSystem<SpikesSystem>();
	mSystemsQueue.appendSystem<SavePoints>();
	mSystemsQueue.appendSystem<Teleport>();
	mSystemsQueue.appendSystem<PuzzleBoulders>();
	mSystemsQueue.appendSystem<Platforms>();
	mSystemsQueue.appendSystem<Pits>();

	mSystemsQueue.appendSystem<Levers>(); // must be after Puzzles

	// must be at the end
	mSystemsQueue.appendSystem<EntityDestroying>();

	initEntityUtil(&mRegistry);
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
	for(auto player : playerView) 
	{
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

