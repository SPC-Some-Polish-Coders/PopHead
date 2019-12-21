#include "staticCollisions.hpp"
#include "ECS/Components/physicsComponents.hpp"
#include "Utilities/profiling.hpp"

namespace ph::system {

	void StaticCollisions::update(float dt)
	{
		PH_PROFILE_FUNCTION();

		auto staticObjects = mRegistry.view<component::BodyRect, component::StaticCollisionBody>(entt::exclude<component::KinematicCollisionBody>);
		auto multiStaticCollisionObjects = mRegistry.view<component::MultiStaticCollisionBody>();
		auto kinematicObjects = mRegistry.view<component::BodyRect, component::KinematicCollisionBody>();
		
		for (auto& kinematicObject : kinematicObjects)
		{
			auto& kinematicBody = kinematicObjects.get<component::BodyRect>(kinematicObject);
			auto& kinematicCollision = kinematicObjects.get<component::KinematicCollisionBody>(kinematicObject);
			kinematicCollision.staticallyMovedByX = false;
			kinematicCollision.staticallyMovedByY = false;

			{
				PH_PROFILE_SCOPE("compute single static collisions");

				// compute single static collisions
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

			{
				PH_PROFILE_SCOPE("compute multi static collision");

				// compute multi static collisions
				for(const auto& multiStaticObject : multiStaticCollisionObjects)
				{
					auto& multiStaticCollisionBody = mRegistry.get<component::MultiStaticCollisionBody>(multiStaticObject);
					for(const FloatRect& staticCollisionBodyRect : multiStaticCollisionBody.rects)
					{
						if(kinematicBody.rect.doPositiveRectsIntersect(staticCollisionBodyRect))
						{
							sf::FloatRect intersection;
							kinematicBody.rect.intersects(staticCollisionBodyRect, intersection);

							if(intersection.width < intersection.height)
							{
								if(kinematicBody.rect.left < staticCollisionBodyRect.left)
									kinematicBody.rect.left -= intersection.width;
								else
									kinematicBody.rect.left += intersection.width;

								kinematicCollision.staticallyMovedByX = true;
							}
							else
							{
								if(kinematicBody.rect.top < staticCollisionBodyRect.top)
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
	}
}
