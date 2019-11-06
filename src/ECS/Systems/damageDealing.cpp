#include "damageDealing.hpp"
#include "ECS/Components/charactersComponents.hpp"
#include "ECS/Components/physicsComponents.hpp"
#include "Utilities/rect.hpp"

namespace ph::system {

	void DamageDealing::update(float seconds)
	{
		//NOTE: Collision checking should be passed to one of the physics systems

		auto playerView = mRegistry.view<component::Player, component::BodyRect, component::Health>();
		auto enemiesView = mRegistry.view<component::BodyRect, component::Damage>();

		for (auto player : playerView)
		{
			const auto& playerBody = mRegistry.get<component::BodyRect>(player);
			auto& playerHealth = mRegistry.get<component::Health>(player);

			for (auto damageDealingEntitiy : enemiesView)
			{
				const auto& enemyBody = mRegistry.get<component::BodyRect>(damageDealingEntitiy);
				if (playerBody.rect.doPositiveRectsIntersect(enemyBody.rect))
				{
					const auto& damage = mRegistry.get<component::Damage>(damageDealingEntitiy);
					playerHealth.healthPoints -= damage.damageDealt;
					//mRegistry.assign_or_replace<component::attackDelayClock>();
				}
			}
		}

	}

}

