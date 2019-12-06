#include "zombieSystem.hpp"
#include "ECS/Components/charactersComponents.hpp"
#include "ECS/Components/aiComponents.hpp"
#include "ECS/Components/physicsComponents.hpp"
#include "ECS/Components/audioComponents.hpp"
#include "AI/aiManager.hpp"
#include "Utilities/random.hpp"
#include "Logs/logs.hpp"

namespace {

sf::Vector2f toDirectionVector(ph::Direction direction)
{
	constexpr float diagonal = 0.7f;

	switch(direction)
	{
	case ph::Direction::east:
		return sf::Vector2f(1.f, 0.f);
	case ph::Direction::west:
		return sf::Vector2f(-1.f, 0.f);
	case ph::Direction::north:
		return sf::Vector2f(0.f, -1.f);
	case ph::Direction::south:
		return sf::Vector2f(0.f, 1.f);
	case ph::Direction::north_east:
		return sf::Vector2f(diagonal, -diagonal);
	case ph::Direction::north_west:
		return sf::Vector2f(-diagonal, -diagonal);
	case ph::Direction::south_east:
		return sf::Vector2f(diagonal, diagonal);
	case ph::Direction::south_west:
		return sf::Vector2f(-diagonal, diagonal);
	case ph::Direction::none:
		return sf::Vector2f();
	default:
		PH_UNEXPECTED_SITUATION("Not all directions were handled in switch");
	}
}

}

namespace ph::system {

ZombieSystem::ZombieSystem(entt::registry& registry, const AIManager* aiManager)
	:System(registry)
	,mAIManager(aiManager)
{
}

void ZombieSystem::update(float dt)
{
	const auto view = mRegistry.view<component::Zombie, component::BodyRect, component::Velocity>(entt::exclude<component::TimeToFadeOut>);
	for(auto zombieEntity : view)
	{
		auto& [zombie, body, velocity] = view.get<component::Zombie, component::BodyRect, component::Velocity>(zombieEntity);

		// make sounds
		zombie.timeFromLastGrowl += dt;
		if(zombie.timeFromLastGrowl > 3.f)
		{
			zombie.timeFromLastGrowl = 0.f;
			int randomNumber = Random::generateNumber(1, 4);
			switch(randomNumber)
			{
				case 1: mRegistry.assign_or_replace<component::SpatialSound>(zombieEntity, "sounds/zombieGrowl1.ogg"); break;
				case 2: mRegistry.assign_or_replace<component::SpatialSound>(zombieEntity, "sounds/zombieGrowl2.ogg"); break;
				case 3: mRegistry.assign_or_replace<component::SpatialSound>(zombieEntity, "sounds/zombieGrowl3.ogg"); break;
				case 4: mRegistry.assign_or_replace<component::SpatialSound>(zombieEntity, "sounds/zombieGrowl4.ogg"); break;
				default:
					PH_UNEXPECTED_SITUATION("Random sound choosing in ZombieSystem failed!");
			}
		}

		// move zombie
		//zombie.timeFromStartingThisMove += dt;
		//if(zombie.pathMode.path.empty())
		//{
		//	zombie.pathMode = mAIManager->getZombiePath(body.rect.getTopLeft());
		//	zombie.timeFromStartingThisMove = 0.f;
		//}

		//constexpr float timeToMoveToAnotherTile = 0.2f;
		//if(zombie.timeFromStartingThisMove > timeToMoveToAnotherTile)
		//{
		//	zombie.timeFromStartingThisMove = 0.f;
		//	Direction currentDirection = zombie.pathMode.path.front();
		//	zombie.pathMode.path.pop_front();
		//	zombie.currentDirectionVector = toDirectionVector(currentDirection);
		//}

		//velocity.dx = 50.f * zombie.currentDirectionVector.x;
		//velocity.dy = 50.f * zombie.currentDirectionVector.y;
	}
}

}

