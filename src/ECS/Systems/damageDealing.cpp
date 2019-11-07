#include "damageDealing.hpp"
#include "ECS/Components/charactersComponents.hpp"
#include "ECS/Components/physicsComponents.hpp"
#include "Utilities/rect.hpp"
#include "Logs/logs.hpp"

namespace ph::system {

	void DamageDealing::update(float seconds)
	{
		handleEnemiesOnDelay(seconds);

		auto playerView = mRegistry.view<component::Player, component::BodyRect, component::Health>();
		auto enemiesView = mRegistry.view<component::BodyRect, component::Damage>(entt::exclude<component::AttackDelayTimer>);

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
					mRegistry.assign<component::AttackDelayTimer>(damageDealingEntitiy);
				}
			}
		}
	}

	void DamageDealing::handleEnemiesOnDelay(const float seconds)
	{
		auto enemiesOnDelay = mRegistry.view<component::AttackDelayTimer>();
		const float timeBetweenAttacks = 200.f;

		for (auto enemy : enemiesOnDelay)
		{
			auto& attackDelayTimer = enemiesOnDelay.get<component::AttackDelayTimer>(enemy);
			attackDelayTimer.delayTime += sf::seconds(seconds);

			if (attackDelayTimer.delayTime.asMilliseconds() > timeBetweenAttacks)
				mRegistry.remove<component::AttackDelayTimer>(enemy);
		}
	}

}
