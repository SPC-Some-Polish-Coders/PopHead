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
			auto &playerBody  = playerView.get<component::BodyRect>(player);
			auto &playerHealth = playerView.get<component::Health>(player);
			
			for (auto item : itemView)
			{
				auto &itemBody = itemView.get<component::BodyRect>(item);
				auto &medkitMed = itemView.get<component::Medkit>(item);

				if (playerBody.rect.doPositiveRectsIntersect(itemBody.rect))
				{
					if (playerHealth.healthPoints + medkitMed.addHealthPoints < playerHealth.maxHealthPoints)
						playerHealth.healthPoints += medkitMed.addHealthPoints;
					else
						playerHealth.healthPoints = playerHealth.maxHealthPoints;

					mRegistry.assign<component::TaggedToDestroy>(item);
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
			auto& playerBody = playerView.get<component::BodyRect>(player);
			auto& playerBullets = playerView.get<component::GunAttacker>(player);

			for (auto item : itemView)
			{
				auto& itemBody = itemView.get<component::BodyRect>(item);
				auto& bulletsToAdd = itemView.get<component::Bullet>(item);

				if (playerBody.rect.doPositiveRectsIntersect(itemBody.rect))
				{
					playerBullets.bullets += bulletsToAdd.numOfBullets;
					mRegistry.assign<component::TaggedToDestroy>(item);
				}
			}
		}
	}
}
