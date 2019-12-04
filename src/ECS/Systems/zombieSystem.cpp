#include "zombieSystem.hpp"
#include "ECS/Components/charactersComponents.hpp"
#include "ECS/Components/audioComponents.hpp"
#include "Utilities/random.hpp"
#include "Logs/logs.hpp"

namespace ph::system {

void ZombieSystem::update(float dt)
{
	const auto view = mRegistry.view<component::Zombie>(entt::exclude<component::TimeToFadeOut>);
	for(auto zombieEntity : view)
	{
		auto& zombieData = view.get<component::Zombie>(zombieEntity);
		zombieData.timeFromLastGrowl += dt;
		if(zombieData.timeFromLastGrowl > 3.f)
		{
			zombieData.timeFromLastGrowl = 0.f;
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
	}
}

}
