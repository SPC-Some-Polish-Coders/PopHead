#include "velocityChangingAreas.hpp"

#include "ECS/Components/objectsComponents.hpp"
#include "ECS/Components/physicsComponents.hpp"
#include "ECS/Components/charactersComponents.hpp"

namespace ph::system {

void VelocityChangingAreas::update(float seconds)
{
	auto velocityChangingAreas = mRegistry.view<component::Area, component::VelocityChangingEffect>();

	for (const auto area : velocityChangingAreas)
	{
	const auto& velocityChangingAreaRect = velocityChangingAreas.get<component::Area>(area);

	auto objectsBeyondAreaView = mRegistry.view<component::CharacterSpeed, component::VelocityChangingEffect>(entt::exclude<component::IsInArea>);
		for (const auto objectBeyondArea : objectsBeyondAreaView)
		{
			const auto& velocityChangingEffect = objectsBeyondAreaView.get<component::VelocityChangingEffect>(objectBeyondArea);
			auto& characterSpeed = objectsBeyondAreaView.get<component::CharacterSpeed>(objectBeyondArea);
			characterSpeed.speed = characterSpeed.speed / velocityChangingEffect.speedMultiplier;

			mRegistry.remove<component::VelocityChangingEffect>(objectBeyondArea);
		}

	auto objectsInVelocityAreaView = mRegistry.view<component::IsInArea, component::BodyRect, component::CharacterSpeed>(entt::exclude<component::VelocityChangingEffect>);
		for (const auto objectInVelocityArea : objectsInVelocityAreaView)
		{
			const auto& characterBody = objectsInVelocityAreaView.get<component::BodyRect>(objectInVelocityArea);

			if (velocityChangingAreaRect.areaBody.contains(sf::Vector2f(characterBody.rect.getCenter().x, characterBody.rect.getBottomLeft().y)))
			{
				const auto& velocityChangingEffect = velocityChangingAreas.get<component::VelocityChangingEffect>(area);
				auto& characterSpeed = objectsInVelocityAreaView.get<component::CharacterSpeed>(objectInVelocityArea);
				characterSpeed.speed = characterSpeed.speed * velocityChangingEffect.speedMultiplier;

				mRegistry.assign_or_replace<component::VelocityChangingEffect>(objectInVelocityArea, velocityChangingEffect.speedMultiplier);
			}
		}
	}

}
}