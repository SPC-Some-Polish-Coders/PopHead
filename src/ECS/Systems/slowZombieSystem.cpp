#include "pch.hpp"
#include "slowZombieSystem.hpp"
#include "ECS/Components/charactersComponents.hpp"
#include "ECS/Components/aiComponents.hpp"
#include "ECS/Components/physicsComponents.hpp"

namespace ph::system {

	void SlowZombieSystem::update(float dt)
	{
		PH_PROFILE_FUNCTION();

		auto slowZombies = mRegistry.view<component::SlowZombieBehavior, component::CharacterSpeed, component::BodyRect, component::CollisionWithPlayer>();
		const auto player = mRegistry.view<component::Player, component::BodyRect>();
		if(player.size() == 0)
			return;

		const auto& playerBody = player.get<component::BodyRect>(*player.begin());

		for(auto& zombie : slowZombies)
		{
			auto& slowZombieBehavior = slowZombies.get<component::SlowZombieBehavior>(zombie);
			auto& zombieSpeed = slowZombies.get<component::CharacterSpeed>(zombie).speed;

			if(slowZombies.get<component::CollisionWithPlayer>(zombie).isCollision)
			{
				slowZombieBehavior.coolDownTimer = component::SlowZombieBehavior::coolDownTime;
				zombieSpeed = component::SlowZombieBehavior::afterAttackSpeed;
				continue;
			}

			if(slowZombieBehavior.coolDownTimer > 0.f)
			{
				slowZombieBehavior.coolDownTimer -= dt;
				continue;
			}

			const auto& zombieBody = slowZombies.get<component::BodyRect>(zombie);
			const auto distance = Math::distanceBetweenPoints(playerBody.center(), zombieBody.center());

			if(distance > component::SlowZombieBehavior::farDistance)
				zombieSpeed = component::SlowZombieBehavior::farFromPlayerSpeed;
			else if(distance > component::SlowZombieBehavior::closeDistance)
				zombieSpeed = component::SlowZombieBehavior::sneakingSpeed;
			else
				zombieSpeed = component::SlowZombieBehavior::attackingSpeed;
		}
	}
}
