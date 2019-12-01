#include "damageDealing.hpp"
#include "ECS/Components/charactersComponents.hpp"
#include "ECS/Components/particleComponents.hpp"
#include "ECS/Components/graphicsComponents.hpp"

namespace ph::system {

	void DamageDealing::update(float seconds)
	{
		dealDamage();
		playDamageAnimation(seconds);
	}

	void DamageDealing::dealDamage() const
	{
		auto view = mRegistry.view<component::DamageTag, component::Health>();
		for (auto entity : view)
		{
			auto& [damageTag, health] = view.get<component::DamageTag, component::Health>(entity);
			health.healthPoints -= damageTag.amountOfDamage;
			mRegistry.remove<component::DamageTag>(entity);
			mRegistry.assign_or_replace<component::DamageAnimation>(entity, 0.14f);
		}
	}

	void DamageDealing::playDamageAnimation(float seconds) const
	{
		auto view = mRegistry.view<component::DamageAnimation, component::Color>();
		for(auto entity : view)
		{
			auto& [damageAnimation, color] = view.get<component::DamageAnimation, component::Color>(entity);
			damageAnimation.timeToEnd -= seconds;
			if(damageAnimation.timeToEnd <= 0.f) {
				mRegistry.remove<component::DamageAnimation>(entity);
				color.color = sf::Color::White;
			}
			else {
				color.color = sf::Color(255, 0, 0);
			}
		}
	}
}
