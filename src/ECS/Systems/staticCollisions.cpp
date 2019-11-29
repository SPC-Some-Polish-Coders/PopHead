#include "staticCollisions.hpp"
#include "ECS/Components/physicsComponents.hpp"

namespace ph::system {

	void StaticCollisions::update(float seconds)
	{
		auto staticObjects = mRegistry.view<component::BodyRect, component::StaticCollisionBody>(entt::exclude<component::KinematicCollisionBody>);
		auto kinematicObjects = mRegistry.view<component::BodyRect, component::KinematicCollisionBody>();
		
		for (auto& kinematicObject : kinematicObjects)
		{
			auto& kinematicBody = kinematicObjects.get<component::BodyRect>(kinematicObject);
			auto& kinematicCollision = kinematicObjects.get<component::KinematicCollisionBody>(kinematicObject);
			kinematicCollision.staticallyMovedByX = false;
			kinematicCollision.staticallyMovedByY = false;

			for (const auto& staticObject : staticObjects)
			{
				const auto& staticBody = staticObjects.get<component::BodyRect>(staticObject);
				
				if (kinematicBody.rect.doPositiveRectsIntersect(staticBody.rect))
				{
					sf::FloatRect intersection;
					kinematicBody.rect.intersects(staticBody.rect, intersection);
					mRegistry.assign_or_replace<component::StaticCollisionBody>(kinematicObject);

					if (intersection.width < intersection.height)
					{
						if (kinematicBody.rect.left < staticBody.rect.left)
							kinematicBody.rect.left -= intersection.width;
						else
							kinematicBody.rect.left += intersection.width;

						kinematicCollision.staticallyMovedByX = true;
					}
					else
					{
						if (kinematicBody.rect.top < staticBody.rect.top)
							kinematicBody.rect.top -= intersection.height;
						else
							kinematicBody.rect.top += intersection.height;

						kinematicCollision.staticallyMovedByY = true;
					}
				}
			}
		}
	}
}
