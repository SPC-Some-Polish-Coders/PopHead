#include "pch.hpp"
#include "hostileCollisions.hpp"
#include "ECS/Components/charactersComponents.hpp"
#include "ECS/Components/graphicsComponents.hpp"
#include "ECS/Components/physicsComponents.hpp"

namespace ph::system {

	void HostileCollisions::update(float dt)
	{
		PH_PROFILE_FUNCTION();

		if(sPause)
			return;

		auto playerView = mRegistry.view<component::Player, component::BodyRect, component::Health, component::Kinematics>();
		auto enemiesView = mRegistry.view<component::BodyRect, component::Damage, component::CollisionWithPlayer>();

		for (auto player : playerView)
		{
			const auto& [playerBody, playerKinematics] = playerView.get<component::BodyRect, component::Kinematics>(player);

			for (auto damageDealingEntitiy : enemiesView)
			{
				auto& playerCollision = enemiesView.get<component::CollisionWithPlayer>(damageDealingEntitiy);
				const auto& enemyBody = enemiesView.get<component::BodyRect>(damageDealingEntitiy);

				if (playerBody.rect.doPositiveRectsIntersect(enemyBody.rect))
				{
					if (playerCollision.isCollision)
						continue;
					playerCollision.isCollision = true;

					if(!godMode)
					{
						const auto& damage = enemiesView.get<component::Damage>(damageDealingEntitiy);
						mRegistry.assign_or_replace<component::DamageTag>(player, damage.damageDealt);
					}

					playerKinematics.vel = playerCollision.pushForce * Math::getUnitVector(playerBody.rect.getCenter() - enemyBody.rect.getCenter());
					playerKinematics.friction = 0.01f;
					playerKinematics.frictionLerpSpeed = 0.04f;

					component::CameraShake shake;
					shake.duration = 1.f;
					shake.magnitude = playerCollision.pushForce / 200.f;
					shake.smooth = false;
					mRegistry.assign_or_replace<component::CameraShake>(player, shake);
				}
				else
				{
					playerCollision.isCollision = false;
				}
			}
		}
	}
}
