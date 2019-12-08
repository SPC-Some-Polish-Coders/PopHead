#include "velocityChangingAreas.hpp"

#include "ECS/Components/objectsComponents.hpp"
#include "ECS/Components/physicsComponents.hpp"
#include "ECS/Components/charactersComponents.hpp"

#include <algorithm>

namespace ph::system {

	void VelocityChangingAreas::update(float seconds)
	{
		handleNewObjectsInsideArea();
		auto objectsInArea = mRegistry.view<component::IsInArea, component::CharacterSpeed>();
		for (const auto objectInArea : objectsInArea)
		{
			const auto& isInArea = objectsInArea.get<component::IsInArea>(objectInArea);
			std::multiset<float> newVelocityEffects = getAllNewMultipliers(isInArea.areas);

			auto& currentVelocityEffects = mRegistry.get<component::VelocityEffects>(objectInArea);
			auto& objectVelocity = objectsInArea.get<component::CharacterSpeed>(objectInArea);

			for (auto divisor : getActualNewMultipliers(currentVelocityEffects.velocityMultipliers, newVelocityEffects))
					objectVelocity.speed /= divisor;

			for (auto factor : getActualNewMultipliers(newVelocityEffects, currentVelocityEffects.velocityMultipliers))
				objectVelocity.speed *= factor;

			mRegistry.replace<component::VelocityEffects>(objectInArea, newVelocityEffects);
		}

		removeVelocityEffectsFromObjectsBeyond();
	}

	void VelocityChangingAreas::handleNewObjectsInsideArea() const
	{
		auto newObjectsInsideAreasView = mRegistry.view<component::IsInArea, component::BodyRect>(entt::exclude <component::VelocityEffects>);
		auto velocityChangingAreasView = mRegistry.view<component::Area, component::AreaVelocityChangingEffect>();

		for (auto newObject : newObjectsInsideAreasView)
		{
			const auto& objectBody = newObjectsInsideAreasView.get<component::BodyRect>(newObject);
			const auto objectBottomCenter = sf::Vector2f(objectBody.rect.getCenter().x - objectBody.rect.width / 2, objectBody.rect.getBottomLeft().y);
			for (auto velocityChangingArea : velocityChangingAreasView)
			{
				const auto& velocityChangingAreaBody = velocityChangingAreasView.get<component::Area>(velocityChangingArea);
				if (velocityChangingAreaBody.areaBody.contains(objectBottomCenter))
				{
					mRegistry.assign<component::VelocityEffects>(newObject);
					return;
				}
			}
		}
	}

	std::multiset<float> VelocityChangingAreas::getAllNewMultipliers(const std::vector<FloatRect>& currentAreas) const
	{
		std::multiset<float> newVelocityEffects;

		auto velocityChangingAreas = mRegistry.view<component::Area, component::AreaVelocityChangingEffect>();
		for (const auto currentArea : currentAreas)
			for (const auto velocityChangingArea : velocityChangingAreas)
			{
				const auto& velocityChangingAreaBody = velocityChangingAreas.get<component::Area>(velocityChangingArea);
				if (currentArea == velocityChangingAreaBody.areaBody)
				{
					const auto& multiplier = velocityChangingAreas.get<component::AreaVelocityChangingEffect>(velocityChangingArea);
					newVelocityEffects.insert(multiplier.areaSpeedMultiplier);
				}
			}

		return newVelocityEffects;
	}

	std::multiset<float> VelocityChangingAreas::getActualNewMultipliers(const std::multiset<float>& minuend, const std::multiset<float>& subtrahend) const
	{
		std::multiset<float> result;
		std::set_difference(minuend.begin(), minuend.end(), subtrahend.begin(), subtrahend.end(), std::inserter(result, result.begin()));
		return result;
	}

	void VelocityChangingAreas::removeVelocityEffectsFromObjectsBeyond() const
	{
		auto objectsOutsideAreas = mRegistry.view<component::CharacterSpeed, component::VelocityEffects>(entt::exclude<component::IsInArea>);
		for (const auto objectOutside : objectsOutsideAreas)
		{
			auto& objectVelocity = objectsOutsideAreas.get<component::CharacterSpeed>(objectOutside);
			const auto& currentVelocityEffects = objectsOutsideAreas.get<component::VelocityEffects>(objectOutside);

			for (const auto& multiplier : currentVelocityEffects.velocityMultipliers)
				objectVelocity.speed /= multiplier;

			mRegistry.remove<component::VelocityEffects>(objectOutside);
		}
	}
}
