#include "pch.hpp"
#include "slowZombieSystem.hpp"
#include "ECS/Components/charactersComponents.hpp"
#include "ECS/Components/aiComponents.hpp"
#include "ECS/Components/physicsComponents.hpp"
#include "ECS/Components/simRegionComponents.hpp"
#include "ECS/entityUtil.hpp"

namespace ph::system {

using namespace component;

void SlowZombieSystem::update(float dt)
{
	PH_PROFILE_FUNCTION();

	if(!isPlayerOnScene()) return;

	auto playerBody = getPlayerBody();

	mRegistry.view<SlowZombieBehavior, InsideSimRegion, CharacterSpeed, BodyRect, CollisionWithPlayer>().each([&]
	(auto& slowZombieBehavior, auto, auto& zombieSpeed, auto zombieBody, auto collisionWithPlayer)
	{
		if(collisionWithPlayer.isCollision)
		{
			slowZombieBehavior.coolDownTimer = SlowZombieBehavior::coolDownTime;
			zombieSpeed.speed = SlowZombieBehavior::afterAttackSpeed;
			return;
		}

		if(slowZombieBehavior.coolDownTimer > 0.f)
		{
			slowZombieBehavior.coolDownTimer -= dt;
			return;
		}

		auto distance = distanceBetweenPoints(playerBody.center(), zombieBody.center());

		if(distance > SlowZombieBehavior::farDistance)
			zombieSpeed.speed = SlowZombieBehavior::farFromPlayerSpeed;
		else if(distance > SlowZombieBehavior::closeDistance)
			zombieSpeed.speed = SlowZombieBehavior::sneakingSpeed;
		else
			zombieSpeed.speed = SlowZombieBehavior::attackingSpeed;
	});
}

}
