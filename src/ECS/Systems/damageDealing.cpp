#include "damageDealing.hpp"
#include "ECS/Components/charactersComponents.hpp"

namespace ph::system {

	void DamageDealing::update(float seconds)
	{
		auto entitiesView = mRegistry.view<component::DamageTag, component::Health>();

		for (auto entity : entitiesView)
		{
			const auto& damageTag = entitiesView.get<component::DamageTag>(entity);
			auto& health = entitiesView.get<component::Health>(entity);
			health.healthPoints -= damageTag.amountOfDamage;
			mRegistry.remove<component::DamageTag>(entity);
		}
	}
}
