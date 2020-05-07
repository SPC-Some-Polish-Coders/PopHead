#include "pch.hpp"
#include "staticCollisions.hpp"
#include "ECS/Components/physicsComponents.hpp"

namespace ph::system {

	void StaticCollisions::update(float dt)
	{
		PH_PROFILE_FUNCTION();

		if(sPause)
			return;

		calculateStaticCollisions();
		calculateKinematicCollisions();
	}

	void StaticCollisions::calculateStaticCollisions()
	{
		auto staticObjects = mRegistry.view<component::BodyRect, component::StaticCollisionBody>();
		auto multiStaticCollisionObjects = mRegistry.view<component::MultiStaticCollisionBody>();
		auto kinematicObjects = mRegistry.view<component::BodyRect, component::KinematicCollisionBody>();

		for(auto& kinematicObject : kinematicObjects)
		{
			auto& kinematicBody = kinematicObjects.get<component::BodyRect>(kinematicObject);
			auto& kinematicCollision = kinematicObjects.get<component::KinematicCollisionBody>(kinematicObject);

			// reset kinematic body
			kinematicCollision.staticallyMovedDown = false;
			kinematicCollision.staticallyMovedLeft = false;
			kinematicCollision.staticallyMovedRight = false;
			kinematicCollision.staticallyMovedUp = false;

			// compute single static collisions
			for(const auto& staticObject : staticObjects)
			{
				const auto& staticBody = staticObjects.get<component::BodyRect>(staticObject);
				handleStaticCollision(staticBody, kinematicBody, kinematicCollision);
			}

			// compute multi static collisions
			for(const auto& multiStaticObject : multiStaticCollisionObjects)
			{
				const auto& multiStaticCollisionBody = mRegistry.get<component::MultiStaticCollisionBody>(multiStaticObject);
				if(intersect(multiStaticCollisionBody.sharedBounds, kinematicBody))
				{
					for(const FloatRect& staticCollisionBodyRect : multiStaticCollisionBody.rects)
					{
						handleStaticCollision(staticCollisionBodyRect, kinematicBody, kinematicCollision);
					}
				}
			}
		}
	}

	void StaticCollisions::handleStaticCollision(const ph::FloatRect& staticBody, ph::FloatRect& kinematicBody, component::KinematicCollisionBody& collision)
	{
		sf::Vector2<short> collisionDirection;

		if(intersect(staticBody, kinematicBody))
		{
			FloatRect intersection;
			kinematicBody.intersects(staticBody, intersection);

			if(intersection.w < intersection.h)
			{
				if(kinematicBody.x < staticBody.x)
				{
					kinematicBody.x -= intersection.w;
					collision.staticallyMovedLeft = true;
				}
				else
				{
					kinematicBody.x += intersection.w;
					collision.staticallyMovedRight = true;
				}
			}
			else
			{
				if(kinematicBody.y < staticBody.y)
				{
					kinematicBody.y -= intersection.h;
					collision.staticallyMovedUp = true;
				}
				else
				{
					kinematicBody.y += intersection.h;
					collision.staticallyMovedDown = true;
				}
			}
		}
		else if(kinematicBody.touch(staticBody, collisionDirection))
		{
			if(collisionDirection.x == -1)
				collision.staticallyMovedLeft = true;
			else if(collisionDirection.x == 1)
				collision.staticallyMovedRight = true;
			else if(collisionDirection.y == -1)
				collision.staticallyMovedUp = true;
			else if(collisionDirection.y == 1)
				collision.staticallyMovedDown = true;
		}
	}

	void StaticCollisions::calculateKinematicCollisions()
	{
		auto kinematicObjects = mRegistry.view<component::BodyRect, component::KinematicCollisionBody>();

		std::vector<entt::entity> pushedLeft;
		std::vector<entt::entity> pushedRight;
		std::vector<entt::entity> pushedUp;
		std::vector<entt::entity> pushedDown;

		for(auto object : kinematicObjects)
		{
			const auto& collision = kinematicObjects.get<component::KinematicCollisionBody>(object);
			if(collision.staticallyMovedLeft)
				pushedLeft.emplace_back(object);
			if(collision.staticallyMovedRight)
				pushedRight.emplace_back(object);
			if(collision.staticallyMovedUp)
				pushedUp.emplace_back(object);
			if(collision.staticallyMovedDown)
				pushedDown.emplace_back(object);
		}

		size_t index = 0;
		while(index != pushedLeft.size())
		{
			const auto& firstRect = kinematicObjects.get<component::BodyRect>(pushedLeft[index]);
			for(auto object : kinematicObjects)
			{
				if(object == pushedLeft[index])
					continue;

				auto& anotherCollisionBody = kinematicObjects.get<component::KinematicCollisionBody>(object);
				if(anotherCollisionBody.staticallyMovedLeft) continue;
				
				auto& anotherRect = kinematicObjects.get<component::BodyRect>(object);
				
				if(intersect(firstRect, anotherRect))
				{
					FloatRect intersection;
					firstRect.intersects(anotherRect, intersection);

					if(intersection.w < intersection.h && anotherRect.x < firstRect.x)
					{
						anotherCollisionBody.staticallyMovedLeft = true;
						anotherRect.x -= intersection.w;
						pushedLeft.emplace_back(object);
					}
				}
			}

			++index;
		}
		
		index = 0;
		while(index != pushedRight.size())
		{
			const auto& firstRect = kinematicObjects.get<component::BodyRect>(pushedRight[index]);
			for(auto object : kinematicObjects)
			{
				if(object == pushedRight[index])
					continue;

				auto& anotherCollisionBody = kinematicObjects.get<component::KinematicCollisionBody>(object);
				if(anotherCollisionBody.staticallyMovedRight) continue;

				auto& anotherRect = kinematicObjects.get<component::BodyRect>(object);

				if(intersect(firstRect, anotherRect))
				{
					FloatRect intersection;
					firstRect.intersects(anotherRect, intersection);

					if(intersection.w < intersection.h && firstRect.x < anotherRect.x)
					{
						anotherCollisionBody.staticallyMovedRight = true;
						anotherRect.x += intersection.w;
						pushedRight.emplace_back(object);
					}
				}
			}

			++index;
		}

		index = 0;
		while(index != pushedUp.size())
		{
			const auto& firstRect = kinematicObjects.get<component::BodyRect>(pushedUp[index]);
			for(auto object : kinematicObjects)
			{
				if(object == pushedUp[index])
					continue;

				auto& anotherCollisionBody = kinematicObjects.get<component::KinematicCollisionBody>(object);
				if(anotherCollisionBody.staticallyMovedUp) continue;

				auto& anotherRect = kinematicObjects.get<component::BodyRect>(object);

				if(intersect(firstRect, anotherRect))
				{
					FloatRect intersection;
					firstRect.intersects(anotherRect, intersection);

					if(intersection.h < intersection.w && anotherRect.y < firstRect.y)
					{
						anotherCollisionBody.staticallyMovedUp = true;
						anotherRect.y -= intersection.h;
						pushedUp.emplace_back(object);
					}
				}
			}

			++index;
		}

		index = 0;
		while(index != pushedDown.size())
		{
			const auto& firstRect = kinematicObjects.get<component::BodyRect>(pushedDown[index]);
			for(auto object : kinematicObjects)
			{
				if(object == pushedDown[index])
					continue;

				auto& anotherCollisionBody = kinematicObjects.get<component::KinematicCollisionBody>(object);
				if(anotherCollisionBody.staticallyMovedDown) continue;

				auto& anotherRect = kinematicObjects.get<component::BodyRect>(object);

				if(intersect(firstRect, anotherRect))
				{
					FloatRect intersection;
					firstRect.intersects(anotherRect, intersection);

					if(intersection.h < intersection.w && firstRect.y < anotherRect.y)
					{
						anotherCollisionBody.staticallyMovedDown = true;
						anotherRect.y += intersection.h;
						pushedDown.emplace_back(object);
					}
				}
			}

			++index;
		}
	}
}
