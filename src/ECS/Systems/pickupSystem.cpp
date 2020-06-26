#include "pch.hpp"
#include "pickupSystem.hpp"
#include "ECS/Components/charactersComponents.hpp"
#include "ECS/Components/physicsComponents.hpp"
#include "ECS/Components/itemComponents.hpp"
#include "ECS/Components/simRegionComponents.hpp"

namespace ph::system {

void PickupItems::update(float dt)
{
	PH_PROFILE_FUNCTION();

	if(sPause) return;

	using namespace component;

	mRegistry.view<Player, BodyRect, BodyCircle, Health, Bullets>().each([&]
	(auto, auto playerBody, auto playerCircle, auto& playerHealth, auto& playerBullets)
	{
		mRegistry.view<Medkit, InsideSimRegion, BodyRect>().each([&]
		(auto medkitEntity, auto medkit, auto, auto medkitBody)
		{
			if(intersect(medkitBody, playerBody, playerCircle))
			{
				if(playerHealth.healthPoints + medkit.addHealthPoints < playerHealth.maxHealthPoints)
					playerHealth.healthPoints += medkit.addHealthPoints;
				else
					playerHealth.healthPoints = playerHealth.maxHealthPoints;

				mRegistry.assign<TaggedToDestroy>(medkitEntity);
			}
		});

		mRegistry.view<BulletBox, InsideSimRegion, Bullets, BodyRect>().each([&]
		(auto bulletBoxEntity, auto, auto, auto bulletBoxBullets, auto bulletBoxBody)
		{
			if(intersect(bulletBoxBody, playerBody, playerCircle))
			{
				playerBullets.numOfPistolBullets += bulletBoxBullets.numOfPistolBullets;
				playerBullets.numOfShotgunBullets += bulletBoxBullets.numOfShotgunBullets;
				mRegistry.assign<TaggedToDestroy>(bulletBoxEntity);
			}
		});
	});
}
}
