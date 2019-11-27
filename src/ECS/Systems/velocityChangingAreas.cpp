#include "velocityChangingAreas.hpp"

#include "ECS/Components/objectsComponents.hpp"
#include "ECS/Components/physicsComponents.hpp"
#include "ECS/Components/charactersComponents.hpp"

namespace ph::system {

	void VelocityChangingAreas::update(float seconds)
	{
		auto objectsInArea = mRegistry.view<component::IsInArea, component::CharacterSpeed>();
		for (const auto objectInArea : objectsInArea)
		{
			const auto& isInArea = objectsInArea.get<component::IsInArea>(objectInArea);
			std::vector<float> newVelocityEffects = getNewMultipliers(isInArea.areas);

			if (!mRegistry.has<component::VelocityEffects>(objectInArea))
				mRegistry.assign<component::VelocityEffects>(objectInArea);

			const auto& currentVelocityEffects = mRegistry.get<component::VelocityEffects>(objectInArea);
			auto& objectVelocity = objectsInArea.get<component::CharacterSpeed>(objectInArea);

			for (auto currentMultiplier : currentVelocityEffects.velocityMultipliers)
				if (std::find(newVelocityEffects.begin(), newVelocityEffects.end(), currentMultiplier) == newVelocityEffects.end())
					objectVelocity.speed /= currentMultiplier;

			for (auto tempEffect : newVelocityEffects)
				if (std::find(currentVelocityEffects.velocityMultipliers.begin(), currentVelocityEffects.velocityMultipliers.end(), tempEffect) == currentVelocityEffects.velocityMultipliers.end())
					objectVelocity.speed *= tempEffect;

			mRegistry.assign_or_replace<component::VelocityEffects>(objectInArea, newVelocityEffects);
		}
		removeVelocityChangingEffects();
	}

	std::vector<float> VelocityChangingAreas::getNewMultipliers(const std::vector<FloatRect>& currentAreas) const
	{
		std::vector<float> newVelocityEffects;
		auto velocityChangingAreas = mRegistry.view<component::Area, component::VelocityChangingEffect>();
		for (const auto velocityChangingArea : velocityChangingAreas)
		{
			const auto& velocityChangingAreaBody = velocityChangingAreas.get<component::Area>(velocityChangingArea);
			for (const auto currentArea : currentAreas)
				if (velocityChangingAreaBody.areaBody == currentArea)
				{
					const auto& multiplier = velocityChangingAreas.get<component::VelocityChangingEffect>(velocityChangingArea);
					newVelocityEffects.push_back(multiplier.speedMultiplier);
				}
		}
		return newVelocityEffects;
	}

	void VelocityChangingAreas::removeVelocityChangingEffects() const
	{
		auto objectsOutside = mRegistry.view<component::CharacterSpeed, component::VelocityEffects>(entt::exclude<component::IsInArea>);
		for (const auto objectOutside : objectsOutside)
		{
			auto& objectVelocity = objectsOutside.get<component::CharacterSpeed>(objectOutside);
			const auto& velAffectionEffects = objectsOutside.get<component::VelocityEffects>(objectOutside);

			for (const auto& var : velAffectionEffects.velocityMultipliers)
				objectVelocity.speed /= var;

			mRegistry.remove<component::VelocityEffects>(objectOutside);
		}
	}
}
