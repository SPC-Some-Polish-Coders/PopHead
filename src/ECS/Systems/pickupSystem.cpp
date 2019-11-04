#include "pickupSystem.hpp"
#include "ECS/Components/charactersComponents.hpp"
#include "ECS/Components/physicsComponents.hpp"
#include "ECS/Components/itemComponents.hpp"

namespace ph::system {

	void PickupMedkit::update(float seconds)
	{
		auto playerView = mRegistry.view<component::Player, component::BodyRect>();
		auto itemView = mRegistry.view<component::Medkit, component::BodyRect>();

		for (auto player : playerView)
		{
			auto &playerBody  = mRegistry.get<component::BodyRect>(player);
			auto &playerHealth = mRegistry.get<component::Health>(player);
			
			for (auto item : itemView)
			{
				auto &itemBody = mRegistry.get<component::BodyRect>(item);
				auto &medkitMed = mRegistry.get<component::Medkit>(item);

				if (playerBody.rect.doPositiveRectsIntersect(itemBody.rect))
				{
					if (playerHealth.healthPoints + medkitMed.addHealthPoints < playerHealth.maxHealthPoints)
					{
						playerHealth.healthPoints += medkitMed.addHealthPoints;
						mRegistry.assign<component::TaggedToDestroy>(item);
					}
					else
					{
						playerHealth.healthPoints = playerHealth.maxHealthPoints;
						mRegistry.assign<component::TaggedToDestroy>(item);
					}
				}
			}
		}
	}

	void PickupBullet::update(float seconds)
	{
		auto playerView = mRegistry.view<component::Player, component::BodyRect>();
		auto itemView = mRegistry.view<component::Medkit, component::BodyRect>();

		for (auto player : playerView)
		{
			auto& playerBody = mRegistry.get<component::BodyRect>(player);
			auto& playerBullets = mRegistry.get<component::GunAttacker>(player);

			for (auto item : itemView)
			{
				auto& itemBody = mRegistry.get<component::BodyRect>(item);
				auto& bulletsToAdd = mRegistry.get<component::Bullet>(item);

				if (playerBody.rect.doPositiveRectsIntersect(itemBody.rect))
				{
					playerBullets.bullets += bulletsToAdd.numOfBullets;
					mRegistry.assign<component::TaggedToDestroy>(item);
				}
			}
		}
	}
}
