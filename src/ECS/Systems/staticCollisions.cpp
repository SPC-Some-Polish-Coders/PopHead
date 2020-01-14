#include "staticCollisions.hpp"
#include "ECS/Components/physicsComponents.hpp"
#include "Utilities/profiling.hpp"

namespace ph::system {

	void StaticCollisions::update(float dt)
	{
		PH_PROFILE_FUNCTION(0);
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
					collision.staticallyMovedLeft = true;
				}
				else
				{
					kinematicBody.left += intersection.width;
					collision.staticallyMovedRight = true;
				}
			}
			else
			{
				if (kinematicBody.top < staticBody.top)
				{
					kinematicBody.top -= intersection.height;
					collision.staticallyMovedUp = true;
				}
				else
				{
					kinematicBody.top += intersection.height;
					collision.staticallyMovedDown = true;
				}
			}
		}
	}

	void StaticCollisions::calculateKinematicCollisions()
	{
		auto kinematicObjects = mRegistry.view<component::BodyRect, component::KinematicCollisionBody>();

		std::vector<entt::entity> pushedLeft;
		std::vector<entt::entity> pushedRight;
		std::vector<entt::entity> pushedUp;
		std::vector<entt::entity> pushedDown;

		for (auto object : kinematicObjects)
		{
			const auto& collision = kinematicObjects.get<component::KinematicCollisionBody>(object);
			if (collision.staticallyMovedLeft)
				pushedLeft.emplace_back(object);
			if (collision.staticallyMovedRight)
				pushedRight.emplace_back(object);
			if (collision.staticallyMovedUp)
				pushedUp.emplace_back(object);
			if (collision.staticallyMovedDown)
				pushedDown.emplace_back(object);
		}

		size_t index = 0;
		while (index != pushedLeft.size())
		{
			const auto& firstRect = kinematicObjects.get<component::BodyRect>(pushedLeft[index]).rect;
			for (auto object : kinematicObjects)
			{
				if (object == pushedLeft[index])
					continue;

				auto& anotherCollisionBody = kinematicObjects.get<component::KinematicCollisionBody>(object);
				if (anotherCollisionBody.staticallyMovedLeft) continue;
				
				auto& anotherRect = kinematicObjects.get<component::BodyRect>(object).rect;
				
				if (firstRect.doPositiveRectsIntersect(anotherRect))
				{
					sf::FloatRect intersection;
					firstRect.intersects(anotherRect, intersection);

					if (intersection.width < intersection.height && anotherRect.left < firstRect.left)
					{
						//anotherCollisionBody.staticallyMovedLeft = true;
						anotherRect.left -= intersection.width;
						pushedLeft.emplace_back(object);
					}
					//handleStaticCollision(firstRect, anotherRect, anotherCollisionBody);
				}
			}

			++index;
		}
	}

	void StaticCollisions::resetKinematicBody(component::KinematicCollisionBody& kinematicBody)
	{
		kinematicBody.staticallyMovedDown = false;
		kinematicBody.staticallyMovedLeft = false;
		kinematicBody.staticallyMovedRight = false;
		kinematicBody.staticallyMovedUp = false;
	}
}
