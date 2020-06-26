#include "pch.hpp"
#include "staticCollisions.hpp"
#include "ECS/Components/physicsComponents.hpp"
#include "ECS/Components/simRegionComponents.hpp"

namespace ph::system {

	void StaticCollisions::update(float dt)
	{
		PH_PROFILE_FUNCTION();

		if(sPause) return;

		calculateStaticCollisions();
		calculateKinematicCollisions();

		pushedLeft.clear();
		pushedRight.clear();
		pushedUp.clear();
		pushedDown.clear();
	}

	using namespace component;

	void StaticCollisions::calculateStaticCollisions()
	{
		auto staticRectObjects = mRegistry.view<BodyRect, StaticCollisionBody, InsideSimRegion>(entt::exclude<BodyCircle>);
		auto staticCircObjects = mRegistry.view<BodyRect, StaticCollisionBody, BodyCircle, InsideSimRegion>();
		auto multiStaticCollisionObjects = mRegistry.view<MultiStaticCollisionBody, InsideSimRegion>();
		auto kinematicRectObjects = mRegistry.view<BodyRect, KinematicCollisionBody, InsideSimRegion>(entt::exclude<BodyCircle>);
		auto kinematicCircObjects = mRegistry.view<BodyRect, KinematicCollisionBody, BodyCircle, InsideSimRegion>();

		for (auto& kinematicObject : kinematicRectObjects)
		{
			auto& kinematicRect = kinematicRectObjects.get<BodyRect>(kinematicObject);
			auto& kinematicBody = kinematicRectObjects.get<KinematicCollisionBody>(kinematicObject);
			resetKinematicBody(kinematicBody);
		
			for (const auto& staticObject : staticRectObjects)
			{
				const auto& staticRect = staticRectObjects.get<BodyRect>(staticObject);
				handleCollision(staticRect, Null, kinematicRect, Null, kinematicObject, kinematicBody);
			}
			for (const auto& staticObject : staticCircObjects)
			{
				const auto& staticRect = staticCircObjects.get<BodyRect>(staticObject);
				const auto& staticCirc = staticCircObjects.get<BodyCircle>(staticObject);
				handleCollision(staticRect, &staticCirc, kinematicRect, Null, kinematicObject, kinematicBody);
			}
			for (const auto& multiStaticObject : multiStaticCollisionObjects)
			{
				const auto& sharedBounds = mRegistry.get<BodyRect>(multiStaticObject);
				const auto& multiStaticCollisionBody = mRegistry.get<MultiStaticCollisionBody>(multiStaticObject);
				if (intersect(sharedBounds, kinematicRect))
				{
					for (const FloatRect& staticRect : multiStaticCollisionBody.rects)
					{
						handleCollision(staticRect, Null, kinematicRect, Null, kinematicObject, kinematicBody);
					}
					for (auto i = 0; i < multiStaticCollisionBody.circles.size(); ++i)
					{
						const auto& staticCircle = multiStaticCollisionBody.circles[i];
						handleCollision({}, &staticCircle, kinematicRect, Null, kinematicObject, kinematicBody);
					}
				}
			}
		}
		for (auto& kinematicObject : kinematicCircObjects)
		{
			auto& kinematicRect = kinematicCircObjects.get<BodyRect>(kinematicObject);
			auto& kinematicCirc = kinematicCircObjects.get<BodyCircle>(kinematicObject);
			auto& kinematicBody = kinematicCircObjects.get<KinematicCollisionBody>(kinematicObject);
			resetKinematicBody(kinematicBody);

			for (const auto& staticObject : staticRectObjects)
			{
				const auto& staticRect = staticRectObjects.get<BodyRect>(staticObject);
				handleCollision(staticRect, Null, kinematicRect, &kinematicCirc, kinematicObject, kinematicBody);
			}
			for (const auto& staticObject : staticCircObjects)
			{
				const auto& staticRect = staticCircObjects.get<BodyRect>(staticObject);
				const auto& staticCirc = staticCircObjects.get<BodyCircle>(staticObject);
				handleCollision(staticRect, &staticCirc, kinematicRect, &kinematicCirc, kinematicObject, kinematicBody);
			}
			for (const auto& multiStaticObject : multiStaticCollisionObjects)
			{
				const auto& sharedBounds = mRegistry.get<BodyRect>(multiStaticObject);
				const auto& multiStaticCollisionBody = mRegistry.get<MultiStaticCollisionBody>(multiStaticObject);
				if (intersect(sharedBounds, kinematicRect))
				{
					for (const FloatRect& staticRect : multiStaticCollisionBody.rects)
					{
						handleCollision(staticRect, Null, kinematicRect, &kinematicCirc, kinematicObject, kinematicBody);
					}
					for (auto i = 0; i < multiStaticCollisionBody.circles.size(); ++i)
					{
						const auto& staticCircle = multiStaticCollisionBody.circles[i];
						handleCollision({}, &staticCircle, kinematicRect, &kinematicCirc, kinematicObject, kinematicBody);
					}
				}
			}
		}
	}

	Vec2 StaticCollisions::handleCollision(const FloatRect& staticRect, const BodyCircle* staticCircle, FloatRect& kinematicRect,
	                                       const BodyCircle* kinematicCircle, entt::entity kinematicEntity, KinematicCollisionBody& kinematicBody)
	{
		auto collisionVector = getCollisionVector(staticRect, staticCircle, kinematicRect, kinematicCircle);
		kinematicRect.pos += collisionVector;

		if (collisionVector.x < 0)
		{
			kinematicBody.staticallyMovedLeft = true;
			pushedLeft.emplace_back(kinematicEntity, kinematicCircle != false);
		}
		if (collisionVector.x > 0)
		{
			kinematicBody.staticallyMovedRight = true;
			pushedRight.emplace_back(kinematicEntity, kinematicCircle != false);
		}
		if (collisionVector.y < 0)
		{
			kinematicBody.staticallyMovedUp = true;
			pushedUp.emplace_back(kinematicEntity, kinematicCircle != false);
		}
		if (collisionVector.y > 0)
		{
			kinematicBody.staticallyMovedDown = true;
			pushedDown.emplace_back(kinematicEntity, kinematicCircle != false);
		}

		return collisionVector;
	}

	void StaticCollisions::calculateKinematicCollisions()
	{
		auto kinematicRectObjects = mRegistry.view<BodyRect, KinematicCollisionBody, InsideSimRegion>(entt::exclude<BodyCircle>);
		auto kinematicCircObjects = mRegistry.view<BodyRect, KinematicCollisionBody, BodyCircle, InsideSimRegion>();

		size_t index = 0;
		while(index != pushedLeft.size())
		{
			BodyRect* firstRect;
			BodyCircle* firstCircle;
			if (pushedLeft[index].isCircle)
			{
				firstRect = &(kinematicCircObjects.get<BodyRect>(pushedLeft[index].entity));
				firstCircle = &(kinematicCircObjects.get<BodyCircle>(pushedLeft[index].entity));
			}
			else
			{
				firstRect = &(kinematicRectObjects.get<BodyRect>(pushedLeft[index].entity));
				firstCircle = Null;
			}

			for (auto secondEntity : kinematicRectObjects)
			{
				if (secondEntity == pushedLeft[index].entity) continue;
				auto& secondRect = kinematicRectObjects.get<BodyRect>(secondEntity);
				auto& secondBody = kinematicRectObjects.get<KinematicCollisionBody>(secondEntity);

				auto collisionVector = getCollisionVector(*firstRect, firstCircle, secondRect, Null);
				if (collisionVector.x + 0.001f < 0)
				{
					secondRect.x += collisionVector.x;
					secondBody.staticallyMovedLeft = true;
					pushedLeft.emplace_back(secondEntity, false);
				}
			}
			for (auto secondEntity : kinematicCircObjects)
			{
				if (secondEntity == pushedLeft[index].entity) continue;
				auto& secondRect = kinematicCircObjects.get<BodyRect>(secondEntity);
				auto& secondCircle = kinematicCircObjects.get<BodyCircle>(secondEntity);
				auto& secondBody = kinematicCircObjects.get<KinematicCollisionBody>(secondEntity);

				auto collisionVector = getCollisionVector(*firstRect, firstCircle, secondRect, &secondCircle);
				if (collisionVector.x + 0.001f < 0)
				{
					secondRect.x += collisionVector.x;
					secondBody.staticallyMovedLeft = true;
					pushedLeft.emplace_back(secondEntity, true);
				}
			}
			++index;
		}

		index = 0;
		while (index != pushedRight.size())
		{
			BodyRect* firstRect;
			BodyCircle* firstCircle;
			if (pushedRight[index].isCircle)
			{
				firstRect = &(kinematicCircObjects.get<BodyRect>(pushedRight[index].entity));
				firstCircle = &(kinematicCircObjects.get<BodyCircle>(pushedRight[index].entity));
			}
			else
			{
				firstRect = &(kinematicRectObjects.get<BodyRect>(pushedRight[index].entity));
				firstCircle = Null;
			}

			for (auto secondEntity : kinematicRectObjects)
			{
				if (secondEntity == pushedRight[index].entity) continue;
				auto& secondRect = kinematicRectObjects.get<BodyRect>(secondEntity);
				auto& secondBody = kinematicRectObjects.get<KinematicCollisionBody>(secondEntity);

				auto collisionVector = getCollisionVector(*firstRect, firstCircle, secondRect, Null);
				if (collisionVector.x - 0.001f > 0)
				{
					secondRect.x += collisionVector.x;
					secondBody.staticallyMovedRight = true;
					pushedRight.emplace_back(secondEntity, false);
				}
			}
			for (auto secondEntity : kinematicCircObjects)
			{
				if (secondEntity == pushedRight[index].entity) continue;
				auto& secondRect = kinematicCircObjects.get<BodyRect>(secondEntity);
				auto& secondCircle = kinematicCircObjects.get<BodyCircle>(secondEntity);
				auto& secondBody = kinematicCircObjects.get<KinematicCollisionBody>(secondEntity);

				auto collisionVector = getCollisionVector(*firstRect, firstCircle, secondRect, &secondCircle);
				if (collisionVector.x - 0.001f > 0)
				{
					secondRect.x += collisionVector.x;
					secondBody.staticallyMovedRight = true;
					pushedRight.emplace_back(secondEntity, true);
				}
			}
			++index;
		}

		index = 0;
		while (index != pushedUp.size())
		{
			BodyRect* firstRect;
			BodyCircle* firstCircle;
			if (pushedUp[index].isCircle)
			{
				firstRect = &(kinematicCircObjects.get<BodyRect>(pushedUp[index].entity));
				firstCircle = &(kinematicCircObjects.get<BodyCircle>(pushedUp[index].entity));
			}
			else
			{
				firstRect = &(kinematicRectObjects.get<BodyRect>(pushedUp[index].entity));
				firstCircle = Null;
			}

			for (auto secondEntity : kinematicRectObjects)
			{
				if (secondEntity == pushedUp[index].entity) continue;
				auto& secondRect = kinematicRectObjects.get<BodyRect>(secondEntity);
				auto& secondBody = kinematicRectObjects.get<KinematicCollisionBody>(secondEntity);

				auto collisionVector = getCollisionVector(*firstRect, firstCircle, secondRect, Null);
				if (collisionVector.y + 0.001f < 0)
				{
					secondRect.y += collisionVector.y;
					secondBody.staticallyMovedUp = true;
					pushedUp.emplace_back(secondEntity, false);
				}
			}
			for (auto secondEntity : kinematicCircObjects)
			{
				if (secondEntity == pushedUp[index].entity) continue;
				auto& secondRect = kinematicCircObjects.get<BodyRect>(secondEntity);
				auto& secondCircle = kinematicCircObjects.get<BodyCircle>(secondEntity);
				auto& secondBody = kinematicCircObjects.get<KinematicCollisionBody>(secondEntity);

				auto collisionVector = getCollisionVector(*firstRect, firstCircle, secondRect, &secondCircle);
				if (collisionVector.y + 0.001f < 0)
				{
					secondRect.y += collisionVector.y;
					secondBody.staticallyMovedUp = true;
					pushedUp.emplace_back(secondEntity, true);
				}
			}
			++index;
		}

		index = 0;
		while (index != pushedDown.size())
		{
			BodyRect* firstRect;
			BodyCircle* firstCircle;
			if (pushedDown[index].isCircle)
			{
				firstRect = &(kinematicCircObjects.get<BodyRect>(pushedDown[index].entity));
				firstCircle = &(kinematicCircObjects.get<BodyCircle>(pushedDown[index].entity));
			}
			else
			{
				firstRect = &(kinematicRectObjects.get<BodyRect>(pushedDown[index].entity));
				firstCircle = Null;
			}

			for (auto secondEntity : kinematicRectObjects)
			{
				if (secondEntity == pushedDown[index].entity) continue;
				auto& secondRect = kinematicRectObjects.get<BodyRect>(secondEntity);
				auto& secondBody = kinematicRectObjects.get<KinematicCollisionBody>(secondEntity);

				auto collisionVector = getCollisionVector(*firstRect, firstCircle, secondRect, Null);
				if (collisionVector.y - 0.001f > 0)
				{
					secondRect.y += collisionVector.y;
					secondBody.staticallyMovedDown = true;
					pushedDown.emplace_back(secondEntity, false);
				}
			}
			for (auto secondEntity : kinematicCircObjects)
			{
				if (secondEntity == pushedDown[index].entity) continue;
				auto& secondRect = kinematicCircObjects.get<BodyRect>(secondEntity);
				auto& secondCircle = kinematicCircObjects.get<BodyCircle>(secondEntity);
				auto& secondBody = kinematicCircObjects.get<KinematicCollisionBody>(secondEntity);

				auto collisionVector = getCollisionVector(*firstRect, firstCircle, secondRect, &secondCircle);
				if (collisionVector.y - 0.001f > 0)
				{
					secondRect.y += collisionVector.y;
					secondBody.staticallyMovedDown = true;
					pushedDown.emplace_back(secondEntity, true);
				}
			}
			++index;
		}
	}

	Vec2 StaticCollisions::getCollisionVector(const FloatRect& staticRect, const BodyCircle* staticCircle, 
	                                          const FloatRect& kinematicRect, const BodyCircle* kinematicCircle) const
	{
		auto rectCircleCollision = [](const FloatRect& firstRect, const BodyCircle& firstCircle, const ph::FloatRect& secondRect) -> Vec2
		{
			auto circleCenter = firstRect.pos + firstCircle.offset;

			if (!(circleCenter.x < secondRect.x || circleCenter.x > secondRect.right()))
			{
				if (circleCenter.y + firstCircle.radius > secondRect.y && circleCenter.y + firstCircle.radius < secondRect.bottom())
				{
					return { 0.f, circleCenter.y + firstCircle.radius - secondRect.y };
				}
				if (circleCenter.y - firstCircle.radius > secondRect.y && circleCenter.y - firstCircle.radius < secondRect.bottom())
				{
					return { 0.f, (circleCenter.y - firstCircle.radius) - secondRect.bottom() };
				}
				return { 0.f, 0.f };
			}
			if (!(circleCenter.y < secondRect.y || circleCenter.y > secondRect.bottom()))
			{
				if (circleCenter.x + firstCircle.radius > secondRect.x && circleCenter.x + firstCircle.radius < secondRect.right())
				{
					return { circleCenter.x + firstCircle.radius - secondRect.x, 0.f };
				}
				if (circleCenter.x - firstCircle.radius > secondRect.x && circleCenter.x - firstCircle.radius < secondRect.right())
				{
					return { (circleCenter.x - firstCircle.radius) - secondRect.right(), 0.f };
				}
				return { 0.f, 0.f };
			}

			auto bottomRightOfRect = secondRect.bottomRight();
			Vec2 rectCorners[4] = { secondRect.pos, {secondRect.x, bottomRightOfRect.y}, {bottomRightOfRect.x, secondRect.y}, bottomRightOfRect };
			float closestDistance = firstCircle.radius;
			size_t closestCornerIndex = 0;

			for (size_t i = 0; i < 4; ++i)
			{
				auto distance = distanceBetweenPoints(circleCenter, rectCorners[i]);
				if (distance < closestDistance)
				{
					closestDistance = distance;
					closestCornerIndex = i;
				}
			}

			if (closestDistance < firstCircle.radius)
			{
				auto intersectionDistance = firstCircle.radius - closestDistance;
				auto distanceVector = rectCorners[closestCornerIndex] - circleCenter;
				distanceVector /= closestDistance;
				distanceVector *= intersectionDistance;
				return distanceVector;
			}
			return { 0.f, 0.f };
		};

		if (staticCircle && kinematicCircle)
		{
			auto staticCircleCenter = staticRect.pos + staticCircle->offset;
			auto kinematicCircleCenter = kinematicRect.pos + kinematicCircle->offset;
			auto centersDistance = distanceBetweenPoints(kinematicCircleCenter, staticCircleCenter);

			if (centersDistance < staticCircle->radius + kinematicCircle->radius)
			{
				auto collisionDistance = staticCircle->radius + kinematicCircle->radius - centersDistance;
				return (kinematicCircleCenter - staticCircleCenter) / centersDistance * collisionDistance;
			}
		}
		else if (staticCircle == Null && kinematicCircle)
		{
			return -rectCircleCollision(kinematicRect, *kinematicCircle, staticRect);
		}
		else if (staticCircle && kinematicCircle == Null)
		{
			return rectCircleCollision(staticRect, *staticCircle, kinematicRect);
		}
		else
		{
			FloatRect intersection;
			staticRect.intersects(kinematicRect, intersection);

			if (intersection.w < intersection.h)
			{
				if (staticRect.x < kinematicRect.x)
					return { intersection.w, 0.f };
				else
					return { -intersection.w, 0.f };
			}
			else if (intersection.h < intersection.w)
			{
				if (staticRect.y < kinematicRect.y)
					return { 0.f, intersection.h };
				else
					return { 0.f, -intersection.h };
			}
		}

		return {};
	}

	void StaticCollisions::resetKinematicBody(KinematicCollisionBody& kinematicBody)
	{
		kinematicBody.staticallyMovedLeft = false;
		kinematicBody.staticallyMovedRight = false;
		kinematicBody.staticallyMovedUp = false;
		kinematicBody.staticallyMovedDown = false;
	}
}
