#include "isObjectInArea.hpp"

#include "ECS/Components/objectsComponents.hpp"
#include "ECS/Components/physicsComponents.hpp"
#include "ECS/Components/charactersComponents.hpp"

namespace ph::system {

	void IsObjectInArea::update(float seconds)
	{
		auto kinematicObjectsView = mRegistry.view<component::KinematicCollisionBody, component::BodyRect>();
		for (const auto kinematicObject : kinematicObjectsView)
		{
			const auto& objectBody = kinematicObjectsView.get<component::BodyRect>(kinematicObject);

			auto areasView = mRegistry.view<component::Area>();
			bool isInAny = false;
			for (const auto area : areasView)
			{
				const auto& areaRect = areasView.get<component::Area>(area);
				if (areaRect.areaBody.contains(sf::Vector2f(objectBody.rect.getCenter().x, objectBody.rect.getBottomLeft().y)))
					isInAny = true;
			}

			if (isInAny)
				mRegistry.assign_or_replace<component::IsInArea>(kinematicObject);
			else
				if (mRegistry.has<component::IsInArea>(kinematicObject))
					mRegistry.remove<component::IsInArea>(kinematicObject);		
		}

		//NOTE: Alternative option! In my opinion it might be less error prone and more efficient

		//auto kinematicObjectsBeyondAreaView = mRegistry.view<component::KinematicCollisionBody, component::BodyRect>(entt::exclude<component::IsInArea>);
		//for (const auto kinematicObject : kinematicObjectsBeyondAreaView)
		//{
		//	const auto& objectBody = kinematicObjectsBeyondAreaView.get<component::BodyRect>(kinematicObject);

		//	auto areasView = mRegistry.view<component::Area>();
		//	bool isInAny = false;
		//	for (const auto area : areasView)
		//	{
		//		const auto& areaRect = areasView.get<component::Area>(area);
		//		if (areaRect.areaBody.contains(objectBody.rect.getBottomLeft()))
		//			isInAny = true;
		//	}

		//	if (isInAny)
		//		mRegistry.assign<component::IsInArea>(kinematicObject);
		//}

		//auto kinematicObjectsWithinAreaView = mRegistry.view<component::KinematicCollisionBody, component::BodyRect, component::IsInArea>();
		//for (const auto kinematicObject : kinematicObjectsWithinAreaView)
		//{
		//	const auto& objectBody = kinematicObjectsWithinAreaView.get<component::BodyRect>(kinematicObject);

		//	auto areasView = mRegistry.view<component::Area>();
		//	bool isInAny = false;
		//	for (const auto area : areasView)
		//	{
		//		const auto& areaRect = areasView.get<component::Area>(area);
		//		if (areaRect.areaBody.contains(objectBody.rect.getBottomLeft()))
		//			isInAny = true;
		//	}

		//	if (!isInAny)
		//		mRegistry.remove<component::IsInArea>(kinematicObject);
		//}

	}
}
