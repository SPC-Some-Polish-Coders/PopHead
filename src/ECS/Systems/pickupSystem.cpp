#include "pickupSystem.hpp"
#include "ECS/Components/charactersComponents.hpp"
#include "ECS/Components/physicsComponents.hpp"
#include"ECS/Components/itemComponents.hpp"


namespace ph::system {

	void PickupMedkit::update(float seconds)
	{
		
		auto playerView = mRegistry.view<component::Player,component::Position,component::Size>();
		auto itemView = mRegistry.view<component::Medkit,component::Position,component::Size>();

		for (auto player : playerView)
		{
			auto &playerPos  = mRegistry.get<component::Position>(player);
			auto &playerSize = mRegistry.get<component::Size>(player);
			auto &playerHealt = mRegistry.get<component::Health>(player);
			
			for (auto item : itemView)
			{
				auto &itemPos = mRegistry.get<component::Position>(item);
				auto &itemSize = mRegistry.get<component::Size>(item);
				auto &medkitMed = mRegistry.get<component::Medkit>(item);

				if (playerPos.x > itemPos.x + itemSize.width ||
					playerPos.x + playerSize.width < itemPos.x ||
					playerPos.y > itemPos.y + itemSize.height ||
					playerPos.y + playerSize.height < itemPos.y)
					return;

				else
					
					if (playerHealt.healthPoints + medkitMed.addHealtPoints < playerHealt.maxHealtPoints)
					{
						playerHealt.healthPoints += medkitMed.addHealtPoints;
						mRegistry.assign<component::TaggedToDestroy>(item);
					}
					else
					{
						playerHealt.healthPoints = playerHealt.maxHealtPoints;
						mRegistry.assign<component::TaggedToDestroy>(item);
					}
			}
			
		}
		
	}

	void PickupBullet::update(float seconds)
	{
		auto playerView = mRegistry.view<component::Player, component::Position, component::Size>();
		auto itemView = mRegistry.view<component::Bullet, component::Position, component::Size>();

		for (auto player : playerView)
		{
			auto &playerPos = mRegistry.get<component::Position>(player);
			auto &playerSize = mRegistry.get<component::Size>(player);
			auto &playerBullets = mRegistry.get<component::GunAttacker>(player);

			for (auto item : itemView)
			{
				auto &itemPos = mRegistry.get<component::Position>(item);
				auto &itemSize = mRegistry.get<component::Size>(item);
				auto &bulletsToAdd = mRegistry.get<component::Bullet>(item);

				if (playerPos.x > itemPos.x + itemSize.width ||
					playerPos.x + playerSize.width < itemPos.x ||
					playerPos.y > itemPos.y + itemSize.height ||
					playerPos.y + playerSize.height < itemPos.y)
					return;

				else
				{
					playerBullets.bullets += bulletsToAdd.numOfBullets;
					mRegistry.assign<component::TaggedToDestroy>(item);

				}
			}
		}
	}
}