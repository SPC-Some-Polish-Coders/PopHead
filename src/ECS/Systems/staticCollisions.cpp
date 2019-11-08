#include "staticCollisions.hpp"
#include "ECS/Components/physicsComponents.hpp"

namespace ph::system {

	void StaticCollisions::update(float seconds)
	{
		auto staticObjects = mRegistry.view<component::BodyRect, component::StaticCollisionBody>();
		auto kinematicObjects = mRegistry.view<component::BodyRect, component::KinematicCollisionBody>();
		
		for (auto& kinematicObject : kinematicObjects)
		{
			auto& kinematicBody = kinematicObjects.get<component::BodyRect>(kinematicObject);
			for (const auto& staticObject : staticObjects)
			{
				const auto& staticBody = staticObjects.get<component::BodyRect>(staticObject);
				
				if (kinematicBody.rect.doPositiveRectsIntersect(staticBody.rect))
				{
					sf::FloatRect intersection;
					kinematicBody.rect.intersects(staticBody.rect, intersection);

					if (intersection.width < intersection.height)
					{
						if (kinematicBody.rect.left < staticBody.rect.left)
							kinematicBody.rect.left -= intersection.width;
						else
							kinematicBody.rect.left += intersection.width;
					}
					else
					{
						if (kinematicBody.rect.top < staticBody.rect.top)
							kinematicBody.rect.top -= intersection.height;
						else
							kinematicBody.rect.top += intersection.height;
					}
				}
			}
		}
	}
}
