#include "pch.hpp"
#include "hostileCollisions.hpp"
#include "ECS/Components/charactersComponents.hpp"
#include "ECS/Components/graphicsComponents.hpp"
#include "ECS/Components/physicsComponents.hpp"

namespace ph::system {

using namespace component;

void HostileCollisions::update(float dt)
{
	PH_PROFILE_FUNCTION();

	if(sPause) return;

	mRegistry.view<BodyRect, BodyCircle, Kinematics, Player, Health>().each([&]
	(auto playerEntity, auto playerBody, auto playerCircle, auto& playerKinematics, auto, auto)
	{
		mRegistry.view<BodyRect, BodyCircle, Damage, CollisionWithPlayer>().each([&]
		(auto enemyBody, auto enemyCircle, auto damage, auto& playerCollision)
		{
			if(intersect(playerBody, playerCircle, enemyBody, enemyCircle) && !playerCollision.isCollision)
			{
				playerCollision.isCollision = true;

				if(!godMode)
					mRegistry.assign_or_replace<DamageTag>(playerEntity, damage.damageDealt);

				auto playerCirclePos = getCirclePos(playerBody, playerCircle);
				auto enemyCirclePos = getCirclePos(enemyBody, enemyCircle);
				playerKinematics.vel = playerCollision.pushForce * Math::getUnitVector(playerCirclePos - enemyCirclePos);
				playerKinematics.friction = 0.01f;
				playerKinematics.frictionLerpSpeed = 0.04f;

				CameraShake shake;
				shake.duration = 1.f;
				shake.magnitude = playerCollision.pushForce / 200.f;
				shake.smooth = false;
				mRegistry.assign_or_replace<CameraShake>(playerEntity, shake);
			}
			else
			{
				playerCollision.isCollision = false;
			}
		});
	});
}

}
