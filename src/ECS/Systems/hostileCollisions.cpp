#include "hostileCollisions.hpp"
#include "ECS/Components/charactersComponents.hpp"
#include "ECS/Components/physicsComponents.hpp"
#include "Utilities/rect.hpp"

namespace ph::system {

	void HostileCollisions::update(float seconds)
	{
		auto playerView = mRegistry.view<component::Player, component::BodyRect, component::Health>();
		auto enemiesView = mRegistry.view<component::BodyRect, component::Damage, component::CollisionWithPlayer>();

		for (auto player : playerView)
		{
			const auto& playerBody = playerView.get<component::BodyRect>(player);
			auto& playerHealth = playerView.get<component::Health>(player);

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
					playerHealth.healthPoints -= damage.damageDealt;
				}
				else
					playerCollision.isCollision = false;
			}
		}
	}
}
