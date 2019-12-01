#include "damageAnimation.hpp"
#include "ECS/Components/charactersComponents.hpp"
#include "ECS/Components/graphicsComponents.hpp"

namespace ph::system {

void DamageAnimation::update(float seconds)
{
	auto entitiesView = mRegistry.view<component::DamageAnimation, component::Color>();

	for(auto entity : entitiesView)
	{
		auto& timeToEnd = mRegistry.get<component::DamageAnimation>(entity).timeToEnd;
		auto& color = mRegistry.get<component::Color>(entity).color;

		timeToEnd -= seconds;
		if(timeToEnd <= 0.f) {
			mRegistry.remove<component::DamageAnimation>(entity);
			color = sf::Color::White;
		}
		else {
			color = sf::Color(255, 0, 0);
		}
		
	}
}

}
