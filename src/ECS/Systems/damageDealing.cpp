#include "damageDealing.hpp"
#include "ECS/Components/charactersComponents.hpp"
#include "ECS/Components/physicsComponents.hpp"
#include "Utilities/rect.hpp"

namespace ph::system {

	void DamageDealing::update(float seconds)
	{
		handleEnemiesOnDelay();

		auto playerView = mRegistry.view<component::Player, component::BodyRect, component::Health>();
		auto enemiesView = mRegistry.view<component::BodyRect, component::Damage>(entt::exclude<component::AttackDelayClock>);

		for (auto player : playerView)
		{
			const auto& playerBody = playerView.get<component::BodyRect>(player);
			auto& playerHealth = playerView.get<component::Health>(player);

			for (auto damageDealingEntitiy : enemiesView)
			{
				const auto& enemyBody = enemiesView.get<component::BodyRect>(damageDealingEntitiy);
				if (playerBody.rect.doPositiveRectsIntersect(enemyBody.rect))
				{
					const auto& damage = enemiesView.get<component::Damage>(damageDealingEntitiy);
					playerHealth.healthPoints -= damage.damageDealt;
					mRegistry.assign<component::AttackDelayClock>(damageDealingEntitiy);
				}
			}
		}
	}

	void DamageDealing::handleEnemiesOnDelay()
	{
		auto enemiesOnDelay = mRegistry.view<component::AttackDelayClock>();
		const float timeBetweenAttacks = 200.f;

		for (auto enemy : enemiesOnDelay)
		{
			const auto& attackDelayClock = enemiesOnDelay.get<component::AttackDelayClock>(enemy);
			if (attackDelayClock.delayClock.getElapsedTime().asMilliseconds() > timeBetweenAttacks)
				mRegistry.remove<component::AttackDelayClock>(enemy);
		}
	}

}
