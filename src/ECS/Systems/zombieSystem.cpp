#include "pch.hpp"
#include "zombieSystem.hpp"
#include "ECS/Components/charactersComponents.hpp"
#include "ECS/Components/aiComponents.hpp"
#include "ECS/Components/physicsComponents.hpp"
#include "ECS/Components/audioComponents.hpp"
#include "ECS/Components/animationComponents.hpp"
#include "AI/aiManager.hpp"
#include "Utilities/direction.hpp"
#include "Utilities/random.hpp"
#include "Utilities/threadPool.hpp"

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

	const auto zombies = mRegistry.view<component::Zombie, component::BodyRect, component::CharacterSpeed, component::Kinematics, component::AnimationData>
		(entt::exclude<component::DeadCharacter>);

	for (auto zombieEntity : zombies)
	{
		auto& [zombie, kinematics, animationData] = zombies.get<component::Zombie, component::Kinematics, component::AnimationData>(zombieEntity);
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
			auto& [zombie, kinematics, animationData] = zombies.get<component::Zombie, component::Kinematics, component::AnimationData>(*begin);
			const auto& [body, speed] = zombies.get<component::BodyRect, component::CharacterSpeed>(*begin);
			
			// move body 
			zombie.timeFromStartingThisMove += dt;
			if (zombie.pathMode.path.empty())
			{
				zombie.pathMode = mAIManager->getZombiePath(body.pos);
				zombie.timeFromStartingThisMove = 0.f;
			}

			if (zombie.timeFromStartingThisMove > zombie.timeToMoveToAnotherTile)
			{
				zombie.timeFromStartingThisMove = 0.f;
				Direction currentDirection = zombie.pathMode.path.front();
				zombie.pathMode.path.pop_front();
				zombie.currentDirectionVector = toDirectionVector(currentDirection);
			}

			kinematics.acceleration = zombie.currentDirectionVector * speed.speed;

			// update animation
			animationData.isPlaying = true;
			if (zombie.currentDirectionVector == PH_NORTH_WEST) {
				animationData.currentStateName = "leftUp";
			}
			else if (zombie.currentDirectionVector == PH_NORTH_EAST) {
				animationData.currentStateName = "rightUp";
			}
			else if (zombie.currentDirectionVector == PH_WEST || zombie.currentDirectionVector == PH_SOUTH_WEST) {
				animationData.currentStateName = "left";
			}
			else if (zombie.currentDirectionVector == PH_EAST || zombie.currentDirectionVector == PH_SOUTH_EAST) {
				animationData.currentStateName = "right";
			}
			else if (zombie.currentDirectionVector == PH_NORTH) {
				animationData.currentStateName = "up";
			}
			else if (zombie.currentDirectionVector == PH_SOUTH) {
				animationData.currentStateName = "down";
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

}
