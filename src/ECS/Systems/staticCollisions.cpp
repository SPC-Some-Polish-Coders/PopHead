#include "staticCollisions.hpp"
#include "ECS/Components/physicsComponents.hpp"
#include "Utilities/profiling.hpp"

namespace ph::system {

	void StaticCollisions::update(float dt)
	{
		PH_PROFILE_FUNCTION();
		calculateStaticCollisions();
		calculateKinematicCollisions();
	}

	void StaticCollisions::calculateStaticCollisions()
	{
		auto staticObjects = mRegistry.view<component::BodyRect, component::StaticCollisionBody>();
		auto multiStaticCollisionObjects = mRegistry.view<component::MultiStaticCollisionBody>();
		auto kinematicObjects = mRegistry.view<component::BodyRect, component::KinematicCollisionBody>();

		for (auto& kinematicObject : kinematicObjects)
		{
			auto& kinematicBody = kinematicObjects.get<component::BodyRect>(kinematicObject);
			auto& kinematicCollision = kinematicObjects.get<component::KinematicCollisionBody>(kinematicObject);
			resetKinematicBody(kinematicCollision);

			// compute single static collisions
			for (const auto& staticObject : staticObjects)
			{
				const auto& staticBody = staticObjects.get<component::BodyRect>(staticObject);
				handleStaticCollision(staticBody.rect, kinematicBody.rect, kinematicCollision);
			}

			// compute multi static collisions
			for (const auto& multiStaticObject : multiStaticCollisionObjects)
			{
				const auto& multiStaticCollisionBody = mRegistry.get<component::MultiStaticCollisionBody>(multiStaticObject);
				if (multiStaticCollisionBody.sharedBounds.doPositiveRectsIntersect(kinematicBody.rect))
				{
					for (const FloatRect& staticCollisionBodyRect : multiStaticCollisionBody.rects)
					{
						handleStaticCollision(staticCollisionBodyRect, kinematicBody.rect, kinematicCollision);
					}
				}
			}
		}
	}

	void StaticCollisions::handleStaticCollision(const ph::FloatRect& staticBody, ph::FloatRect& kinematicBody, component::KinematicCollisionBody& collision)
	{
		if (staticBody.doPositiveRectsIntersect(kinematicBody))
		{
			sf::FloatRect intersection;
			kinematicBody.intersects(staticBody, intersection);

			if (intersection.width < intersection.height)
			{
				if (kinematicBody.left < staticBody.left)
				{
					kinematicBody.left -= intersection.width;
					collision.staticallyMovedLeft;
				}
				else
				{
					kinematicBody.left += intersection.width;
					collision.staticallyMovedRight;
				}
			}
			else
			{
				if (kinematicBody.top < staticBody.top)
				{
					kinematicBody.top -= intersection.height;
					collision.staticallyMovedUp;
				}
				else
				{
					kinematicBody.top += intersection.height;
					collision.staticallyMovedDown;
				}
			}
		}
	}

	void StaticCollisions::calculateKinematicCollisions()
	{
	}

	void StaticCollisions::resetKinematicBody(component::KinematicCollisionBody& kinematicBody)
	{
		kinematicBody.staticallyMovedDown = false;
		kinematicBody.staticallyMovedLeft = false;
		kinematicBody.staticallyMovedRight = false;
		kinematicBody.staticallyMovedUp = false;
	}
}
