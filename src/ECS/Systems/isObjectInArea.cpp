#include "isObjectInArea.hpp"

#include "ECS/Components/objectsComponents.hpp"
#include "ECS/Components/physicsComponents.hpp"
#include "ECS/Components/charactersComponents.hpp"

#include <iostream>

namespace ph::system {

void IsObjectInArea::update(float dt)
{
	auto areasView = mRegistry.view<component::Area>();
	auto kinematicObjectsOutsideAreasView = mRegistry.view<component::KinematicCollisionBody, component::BodyRect>(entt::exclude<component::IsInArea>);
	auto kinematicObjectsInsideAreasView = mRegistry.view<component::KinematicCollisionBody, component::BodyRect, component::IsInArea>();
	for (auto area : areasView)
	{
		const auto& areaBody = areasView.get<component::Area>(area);
		for (auto kinematicObject : kinematicObjectsOutsideAreasView)
		{
			const auto& objectBody = kinematicObjectsOutsideAreasView.get<component::BodyRect>(kinematicObject);
			if (areaBody.areaBody.contains(objectBody.rect.getCenter()))
			{
				mRegistry.assign<component::IsInArea>(kinematicObject);
				continue;
			}
		}
	}

	for (auto kinematicObject : kinematicObjectsInsideAreasView)
	{
		const auto& objectBody = kinematicObjectsInsideAreasView.get<component::BodyRect>(kinematicObject);
		bool isInArea = false;

		for (auto area : areasView)
		{
			const auto& areaBody = areasView.get<component::Area>(area);
			if (areaBody.areaBody.contains(objectBody.rect.getCenter()))
			{
				isInArea = true;
				break;
			}
		}

		if(!isInArea)
			mRegistry.remove<component::IsInArea>(kinematicObject);
	}
}

}

