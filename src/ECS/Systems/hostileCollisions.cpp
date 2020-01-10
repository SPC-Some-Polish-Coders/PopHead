#include "hostileCollisions.hpp"
#include "ECS/Components/charactersComponents.hpp"
#include "ECS/Components/physicsComponents.hpp"
#include "Utilities/rect.hpp"
#include "Utilities/profiling.hpp"
#include "Utilities/math.hpp"

namespace ph::system {

	void HostileCollisions::update(float dt)
	{
		PH_PROFILE_FUNCTION(0);

		auto playerView = mRegistry.view<component::Player, component::BodyRect, component::Health, component::PushingForces>();
		auto enemiesView = mRegistry.view<component::BodyRect, component::Damage, component::CollisionWithPlayer>();

		for (auto player : playerView)
		{
			const auto& playerBody = playerView.get<component::BodyRect>(player);
			auto& playerPushingForces = playerView.get<component::PushingForces>(player);

			for (auto damageDealingEntitiy : enemiesView)
			{
				auto& playerCollision = enemiesView.get<component::CollisionWithPlayer>(damageDealingEntitiy);
				const auto& enemyBody = enemiesView.get<component::BodyRect>(damageDealingEntitiy);

				if (playerBody.rect.doPositiveRectsIntersect(enemyBody.rect))
				{
					if (playerCollision.isCollision)
						continue;
					playerCollision.isCollision = true;

					const auto& damage = enemiesView.get<component::Damage>(damageDealingEntitiy);
					mRegistry.assign<component::DamageTag>(player, damage.damageDealt);

					playerPushingForces.vel = playerCollision.pushForce * Math::getUnitVector(playerBody.rect.getCenter() - enemyBody.rect.getCenter());
					playerPushingForces.friction = 1.f;
				}
				else
					playerCollision.isCollision = false;
			}
		}
	}
}
