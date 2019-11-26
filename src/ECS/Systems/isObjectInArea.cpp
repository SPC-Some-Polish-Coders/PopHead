#include "isObjectInArea.hpp"

#include "ECS/Components/objectsComponents.hpp"
#include "ECS/Components/physicsComponents.hpp"
#include "ECS/Components/charactersComponents.hpp"

namespace ph::system {

	void IsObjectInArea::update(float seconds)
	{
		auto areasView = mRegistry.view<component::Area>();
		for (const auto area : areasView)
		{
			const auto& areaRect = areasView.get<component::Area>(area);

			auto objectsInAreasView = mRegistry.view<component::IsInArea, component::BodyRect>();
			for (const auto objectInArea : objectsInAreasView)
			{
				const auto& objectInAreaBody = objectsInAreasView.get<component::BodyRect>(objectInArea);
				if (!areaRect.areaBody.contains(objectInAreaBody.rect.getBottomLeft()))
					mRegistry.remove<component::IsInArea>(objectInArea);
			}

			auto kinematicObjectsView = mRegistry.view<component::KinematicCollisionBody, component::BodyRect>(entt::exclude<component::IsInArea>);
			for (const auto kinematicObject : kinematicObjectsView)
			{
				const auto& kinematicObjectBody = kinematicObjectsView.get<component::BodyRect>(kinematicObject);
				if (areaRect.areaBody.contains(kinematicObjectBody.rect.getBottomLeft()))
					mRegistry.assign<component::IsInArea>(kinematicObject);
			}
		}
	}
}
