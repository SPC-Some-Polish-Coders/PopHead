#include "pickupSystem.hpp"
#include "ECS/Components/charactersComponents.hpp"
#include "ECS/Components/physicsComponents.hpp"
#include "ECS/Components/itemComponents.hpp"
#include "Utilities/profiling.hpp"

namespace ph::system {

	void PickupItems::update(float dt)
	{
		PH_PROFILE_FUNCTION(0);

		auto players = mRegistry.view<component::Player, component::BodyRect, component::Health, component::Bullets>();
		auto medkits = mRegistry.view<component::Medkit, component::BodyRect>();
		auto bulletBoxes = mRegistry.view<component::BulletBox, component::Bullets, component::BodyRect>();

		for (auto player : players)
		{
			const auto& playerBody  = players.get<component::BodyRect>(player);
			auto& [playerHealth, playerBullets] = players.get<component::Health, component::Bullets>(player);
			
			for (auto medkitEntity : medkits)
			{
				const auto& [medkit, medkitBody] = medkits.get<component::Medkit, component::BodyRect>(medkitEntity);

				if (playerBody.rect.doPositiveRectsIntersect(medkitBody.rect)) {
					if (playerHealth.healthPoints + medkit.addHealthPoints < playerHealth.maxHealthPoints)
						playerHealth.healthPoints += medkit.addHealthPoints;
					else
						playerHealth.healthPoints = playerHealth.maxHealthPoints;
					mRegistry.assign<component::TaggedToDestroy>(medkitEntity);
				}
			}

			for (auto bulletBoxEntity : bulletBoxes)
			{
				const auto& [bulletBoxBullets, bulletBoxBody] = bulletBoxes.get<component::Bullets, component::BodyRect>(bulletBoxEntity);

				if (playerBody.rect.doPositiveRectsIntersect(bulletBoxBody.rect)) {
					playerBullets.numOfPistolBullets += bulletBoxBullets.numOfPistolBullets;
					playerBullets.numOfShotgunBullets += bulletBoxBullets.numOfShotgunBullets;
					mRegistry.assign<component::TaggedToDestroy>(bulletBoxEntity);
				}
			}
		}
	}
}
