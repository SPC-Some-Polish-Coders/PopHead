#include "isObjectInArea.hpp"

#include "ECS/Components/objectsComponents.hpp"
#include "ECS/Components/physicsComponents.hpp"
#include "ECS/Components/charactersComponents.hpp"

#include <algorithm>
#include <iostream>

#include "Utilities/rect.hpp"


namespace ph::system {

	bool IsObjectInArea::isSizeInVector(const std::vector<FloatRect>& sizes, const FloatRect& sizeValue) const
	{
		return std::find(sizes.begin(), sizes.end(), sizeValue) != sizes.end();
	}

	void IsObjectInArea::update(float seconds)
	{
		//Checks whether any object has entered any of the areas
		auto kinematicObjectsBeyondAreaView = mRegistry.view<component::KinematicCollisionBody, component::BodyRect>(entt::exclude<component::IsInArea>);
		for (const auto kinematicObject : kinematicObjectsBeyondAreaView)
		{
			const auto& objectBody = kinematicObjectsBeyondAreaView.get<component::BodyRect>(kinematicObject);

			auto areasView = mRegistry.view<component::Area>();
			bool isInAny = false;
			for (const auto area : areasView)
			{
				const auto& areaRect = areasView.get<component::Area>(area);
				if (areaRect.areaBody.contains(objectBody.rect.getBottomLeft()))
					isInAny = true;
			}

			if (isInAny)
				mRegistry.assign<component::IsInArea>(kinematicObject);
		}

		auto kinematicObjectsWithinAreaView = mRegistry.view<component::KinematicCollisionBody, component::BodyRect, component::IsInArea>();
		for (const auto kinematicObject : kinematicObjectsWithinAreaView)
		{
			const auto& objectBody = kinematicObjectsWithinAreaView.get<component::BodyRect>(kinematicObject);
			auto& isInArea = kinematicObjectsWithinAreaView.get<component::IsInArea>(kinematicObject);

			auto areasView = mRegistry.view<component::Area>();
			for (const auto area : areasView)
			{
				const auto& areaBody = areasView.get<component::Area>(area);
				if (areaBody.areaBody.contains(objectBody.rect.getBottomLeft()) && !isSizeInVector(isInArea.areaSizes, areaBody.areaBody))
					isInArea.areaSizes.push_back(areaBody.areaBody);
			}

			for (auto it = isInArea.areaSizes.begin(); it != isInArea.areaSizes.end(); ++it)
				if (!it->contains(objectBody.rect.getBottomLeft()))
				{
					isInArea.areaSizes.erase(it);
					break;
				}

			if (isInArea.areaSizes.empty())
				mRegistry.remove<component::IsInArea>(kinematicObject);
		}
	}

	}

