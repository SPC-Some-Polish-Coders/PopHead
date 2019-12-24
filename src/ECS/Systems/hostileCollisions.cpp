#include "hostileCollisions.hpp"
#include "ECS/Components/charactersComponents.hpp"
#include "ECS/Components/physicsComponents.hpp"
#include "Utilities/rect.hpp"
#include "Utilities/profiling.hpp"

namespace ph::system {

	void HostileCollisions::update(float dt)
	{
		PH_PROFILE_FUNCTION();

		auto playerView = mRegistry.view<component::Player, component::BodyRect, component::Health>();
		auto enemiesView = mRegistry.view<component::BodyRect, component::Damage, component::CollisionWithPlayer>();

		for (auto player : playerView)
		{
			const auto& playerBody = playerView.get<component::BodyRect>(player);

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
				}
				else
					playerCollision.isCollision = false;
			}
		}
	}
}
