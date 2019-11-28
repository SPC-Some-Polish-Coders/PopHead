#include "isObjectInArea.hpp"

#include "ECS/Components/objectsComponents.hpp"
#include "ECS/Components/physicsComponents.hpp"
#include "ECS/Components/charactersComponents.hpp"

#include <algorithm>

namespace ph::system {

void IsObjectInArea::update(float seconds)
{
	handleObjectsOutsideAreas();
	handleObjectsInsideAreas();
}

void IsObjectInArea::handleObjectsOutsideAreas() const
{
	auto kinematicObjectsBeyondAreaView = mRegistry.view<component::KinematicCollisionBody, component::BodyRect>(entt::exclude<component::IsInArea>);
	for (const auto kinematicObject : kinematicObjectsBeyondAreaView)
	{
		const auto& objectBody = kinematicObjectsBeyondAreaView.get<component::BodyRect>(kinematicObject);
		auto objectBottomCenter = sf::Vector2f(objectBody.rect.getCenter().x - objectBody.rect.width / 2, objectBody.rect.getBottomLeft().y);

		auto areasView = mRegistry.view<component::Area>();
		for (const auto area : areasView)
		{
			const auto& areaRect = areasView.get<component::Area>(area);
			if (areaRect.areaBody.contains(objectBottomCenter))
			{
				mRegistry.assign_or_replace<component::IsInArea>(kinematicObject);
				break;
			}
		}
	}
}

void IsObjectInArea::handleObjectsInsideAreas() const
{
	auto kinematicObjectsWithinAreaView = mRegistry.view<component::KinematicCollisionBody, component::BodyRect, component::IsInArea>();
	for (const auto kinematicObject : kinematicObjectsWithinAreaView)
	{
		auto& isInAreaComponent = kinematicObjectsWithinAreaView.get<component::IsInArea>(kinematicObject);
		const auto& objectBody = kinematicObjectsWithinAreaView.get<component::BodyRect>(kinematicObject);
		const auto objectBottomCenter = sf::Vector2f(objectBody.rect.getCenter().x - objectBody.rect.width / 2, objectBody.rect.getBottomLeft().y);

		auto areasView = mRegistry.view<component::Area>();
		for (const auto area : areasView)
		{
			const auto& areaBody = areasView.get<component::Area>(area);
			auto position = positionInVector(isInAreaComponent.areas, areaBody.areaBody);
			bool isAlreadyInVector = position != isInAreaComponent.areas.end();
			bool isContainedInArea = areaBody.areaBody.contains(objectBottomCenter);

			if (isContainedInArea && !isAlreadyInVector)
				isInAreaComponent.areas.emplace_back(areaBody.areaBody);
			else if (!isContainedInArea && isAlreadyInVector)
				isInAreaComponent.areas.erase(position);
		}

		if (isInAreaComponent.areas.empty())
			mRegistry.remove<component::IsInArea>(kinematicObject);
	}
}

auto IsObjectInArea::positionInVector(const std::vector<FloatRect>& sizes, const FloatRect& sizeValue) const -> std::vector<FloatRect>::const_iterator
{
	return std::find(sizes.begin(), sizes.end(), sizeValue);
}

}

