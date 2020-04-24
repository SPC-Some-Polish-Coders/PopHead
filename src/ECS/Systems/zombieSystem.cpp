#include "zombieSystem.hpp"
#include "ECS/Components/charactersComponents.hpp"
#include "ECS/Components/aiComponents.hpp"
#include "ECS/Components/physicsComponents.hpp"
#include "ECS/Components/audioComponents.hpp"
#include "ECS/Components/animationComponents.hpp"
#include "AI/aiManager.hpp"
#include "Utilities/direction.hpp"
#include "Utilities/random.hpp"
#include "Utilities/profiling.hpp"
#include "Utilities/threadPool.hpp"
#include "Utilities/math.hpp"
#include "Logs/logs.hpp"

namespace {
	
sf::Vector2f toDirectionVector(ph::Direction direction)
{
	switch(direction)
	{
	case ph::Direction::east: return PH_EAST;
	case ph::Direction::west: return PH_WEST;
	case ph::Direction::north: return PH_NORTH;
	case ph::Direction::south: return PH_SOUTH;
	case ph::Direction::north_east: return PH_NORTH_EAST;
	case ph::Direction::north_west: return PH_NORTH_WEST;
	case ph::Direction::south_east: return PH_SOUTH_EAST;
	case ph::Direction::south_west: return PH_SOUTH_WEST;
	case ph::Direction::none: return PH_NULL_DIRECTION;
	default:
		PH_UNEXPECTED_SITUATION("Not all directions were handled in switch");
	}
}

}

namespace ph::system {

ZombieSystem::ZombieSystem(entt::registry& registry, const AIManager* aiManager, ThreadPool& threadPool)
	:System(registry)
	,mAIManager(aiManager)
	,mThreadPool(threadPool)
{
}

void ZombieSystem::update(float dt)
{
	PH_PROFILE_FUNCTION();

	if(sPause || freezeZombies)
		return;

	const auto zombies = mRegistry.view<component::Zombie, component::BodyRect, component::CharacterSpeed, component::Velocity, component::AnimationData>
		(entt::exclude<component::DeadCharacter>);

	for (auto zombieEntity : zombies)
	{
		auto& [zombie, velocity, animationData] = zombies.get<component::Zombie, component::Velocity, component::AnimationData>(zombieEntity);
		const auto& [body, speed] = zombies.get<component::BodyRect, component::CharacterSpeed>(zombieEntity);

		// make sounds
		zombie.timeFromLastGrowl += dt;
		if (zombie.timeFromLastGrowl > 3.f)
		{
			zombie.timeFromLastGrowl = 0.f;
			int randomNumber = Random::generateNumber(1, 4);
			switch (randomNumber)
			{
			case 1: mRegistry.assign_or_replace<component::SpatialSound>(zombieEntity, "sounds/zombieGrowl1.ogg"); break;
			case 2: mRegistry.assign_or_replace<component::SpatialSound>(zombieEntity, "sounds/zombieGrowl2.ogg"); break;
			case 3: mRegistry.assign_or_replace<component::SpatialSound>(zombieEntity, "sounds/zombieGrowl3.ogg"); break;
			case 4: mRegistry.assign_or_replace<component::SpatialSound>(zombieEntity, "sounds/zombieGrowl4.ogg"); break;
			default:
				PH_UNEXPECTED_SITUATION("Random sound choosing in ZombieSystem failed!");
			}
		}
	}
	
	auto numOfZombies = zombies.size();
	numOfZombies /= 2;

	auto begin = zombies.begin();
	auto secondBegin = begin;
	for (size_t i = 0; i < numOfZombies && secondBegin != zombies.end(); ++i)
		++secondBegin;

	auto task = [&zombies, dt, this](decltype(zombies)::iterator_type begin, decltype(zombies)::iterator_type end) {

		for (; begin != end; ++begin)
		{
			auto& [zombie, velocity, animationData] = zombies.get<component::Zombie, component::Velocity, component::AnimationData>(*begin);
			const auto& [body, speed] = zombies.get<component::BodyRect, component::CharacterSpeed>(*begin);
			
			// move body 
			zombie.timeFromStartingThisMove += dt;
			if (zombie.pathMode.path.empty())
			{
				zombie.pathMode = mAIManager->getZombiePath(body.rect.getTopLeft());
				zombie.timeFromStartingThisMove = 0.f;
			}

			if (zombie.timeFromStartingThisMove > zombie.timeToMoveToAnotherTile)
			{
				zombie.timeFromStartingThisMove = 0.f;
				Direction currentDirection = zombie.pathMode.path.front();
				zombie.pathMode.path.pop_front();
				zombie.currentDirectionVector = toDirectionVector(currentDirection);
			}

			velocity.dx = zombie.currentDirectionVector.x * speed.speed;
			velocity.dy = zombie.currentDirectionVector.y * speed.speed;

			// update animation
			if (zombie.currentDirectionVector == PH_NORTH_WEST) {
				animationData.currentStateName = "leftUp";
				animationData.isPlaying = true;
			}
			else if (zombie.currentDirectionVector == PH_NORTH_EAST) {
				animationData.currentStateName = "rightUp";
				animationData.isPlaying = true;
			}
			else if (zombie.currentDirectionVector == PH_WEST || zombie.currentDirectionVector == PH_SOUTH_WEST) {
				animationData.currentStateName = "left";
				animationData.isPlaying = true;
			}
			else if (zombie.currentDirectionVector == PH_EAST || zombie.currentDirectionVector == PH_SOUTH_EAST) {
				animationData.currentStateName = "right";
				animationData.isPlaying = true;
			}
			else if (zombie.currentDirectionVector == PH_NORTH) {
				animationData.currentStateName = "up";
				animationData.isPlaying = true;
			}
			else if (zombie.currentDirectionVector == PH_SOUTH) {
				animationData.currentStateName = "down";
				animationData.isPlaying = true;
			}
			else {
				animationData.isPlaying = false;
			}
		}
	};

	auto future = mThreadPool.addTask([task, begin, secondBegin]() {
		task(begin, secondBegin);
		});
	task(secondBegin, zombies.end());

	future.get();
}

void SlowZombieSystem::update(float dt)
{
	PH_PROFILE_FUNCTION();

	auto slowZombies = mRegistry.view<component::SlowZombieBehavior, component::CharacterSpeed, component::BodyRect, component::CollisionWithPlayer>();
	const auto player = mRegistry.view<component::Player, component::BodyRect>();
	if (player.size() == 0)
		return;

	const auto& playerRect = player.get<component::BodyRect>(*player.begin()).rect;

	for (auto& zombie : slowZombies)
	{
		auto& slowZombieBehavior = slowZombies.get<component::SlowZombieBehavior>(zombie);
		auto& zombieSpeed = slowZombies.get<component::CharacterSpeed>(zombie).speed;

		if (slowZombies.get<component::CollisionWithPlayer>(zombie).isCollision)
		{
			slowZombieBehavior.coolDownTimer = component::SlowZombieBehavior::coolDownTime;
			zombieSpeed = component::SlowZombieBehavior::afterAttackSpeed;
			continue;
		}

		if (slowZombieBehavior.coolDownTimer > 0.f)
		{
			slowZombieBehavior.coolDownTimer -= dt;
			continue;
		}

		const auto& zombieRect = slowZombies.get<component::BodyRect>(zombie).rect;
		const auto distance = Math::distanceBetweenPoints(playerRect.getCenter(), zombieRect.getCenter());

		if (distance > component::SlowZombieBehavior::farDistance)
			zombieSpeed = component::SlowZombieBehavior::farFromPlayerSpeed;
		else if (distance > component::SlowZombieBehavior::closeDistance)
			zombieSpeed = component::SlowZombieBehavior::sneakingSpeed;
		else
			zombieSpeed = component::SlowZombieBehavior::attackingSpeed;
	}
}

}
