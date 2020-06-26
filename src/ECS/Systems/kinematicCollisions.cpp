#include "pch.hpp"
#include "kinematicCollisions.hpp"
#include "ECS/Components/physicsComponents.hpp"
#include "ECS/Components/simRegionComponents.hpp"

namespace ph::system {

	using namespace component;

	void KinematicCollisions::update(float dt)
	{
		PH_PROFILE_FUNCTION();

		if (sPause) return;

		auto kinematicRectObjects = mRegistry.view<KinematicCollisionBody, InsideSimRegion, BodyRect>(entt::exclude<BodyCircle>);
		auto kinematicCircObjects = mRegistry.view<KinematicCollisionBody, InsideSimRegion, BodyCircle, BodyRect>();

		for (auto current = kinematicRectObjects.begin(); current != kinematicRectObjects.end(); ++current)
		{
			auto& currentBody = kinematicRectObjects.get<BodyRect>(*current);

			auto another = current;
			++another;

			// rect-on-rect collisions
			for (; another != kinematicRectObjects.end(); ++another)
			{
				auto& anotherBody = kinematicRectObjects.get<BodyRect>(*another);

				if (intersect(currentBody, anotherBody))
				{
					FloatRect intersection;
					currentBody.intersects(anotherBody, intersection);

					if (intersection.w < intersection.h)
					{
						auto halfWidth = intersection.w / 2.f;
						if (currentBody.x < anotherBody.x)
						{
							currentBody.x -= halfWidth;
							anotherBody.x += halfWidth;
						}
						else
						{
							currentBody.x += halfWidth;
							anotherBody.x -= halfWidth;
						}
					}
					else
					{
						auto halfHeight = intersection.h / 2.f;
						if (currentBody.y < anotherBody.y)
						{
							currentBody.y -= halfHeight;
							anotherBody.y += halfHeight;
						}
						else
						{
							currentBody.y += halfHeight;
							anotherBody.y -= halfHeight;
						}
					}
				}
			}
		
			// rect-on-circle collisions
			for (auto& circObject : kinematicCircObjects)
			{
				auto& circRect = kinematicCircObjects.get<BodyRect>(circObject);
				auto& circCircle = kinematicCircObjects.get<BodyCircle>(circObject);
				auto circleCenter = circRect.pos + circCircle.offset;

				if (!(circleCenter.x < currentBody.x || circleCenter.x > currentBody.right()))
				{
					if (circleCenter.y + circCircle.radius > currentBody.y && circleCenter.y + circCircle.radius < currentBody.bottom())
					{
						auto offset = circleCenter.y + circCircle.radius - currentBody.y;
						offset /= 2.f;
						circRect.y -= offset;
						currentBody.y += offset;
						continue;
					}
					if (circleCenter.y - circCircle.radius > currentBody.y && circleCenter.y - circCircle.radius < currentBody.bottom())
					{
						auto offset = currentBody.bottom() - (circleCenter.y - circCircle.radius);
						offset /= 2.f;
						circRect.y += offset;
						currentBody.y -= offset;
						continue;
					}
					continue;
				}
				if (!(circleCenter.y < currentBody.y || circleCenter.y > currentBody.bottom()))
				{
					if (circleCenter.x + circCircle.radius > currentBody.x && circleCenter.x + circCircle.radius < currentBody.right())
					{
						auto offset = circleCenter.x + circCircle.radius - currentBody.x;
						offset /= 2.f;
						circRect.x -= offset;
						currentBody.x += offset;
						continue;
					}
					if (circleCenter.x - circCircle.radius > currentBody.x && circleCenter.x - circCircle.radius < currentBody.right())
					{
						auto offset = currentBody.right() - (circleCenter.x - circCircle.radius);
						offset /= 2.f;
						circRect.x += offset;
						currentBody.x -= offset;
						continue;
					}
					continue;
				}

				auto bottomRightOfRect = currentBody.bottomRight();
				Vec2 rectCorners[4] = { currentBody.pos, {currentBody.x, bottomRightOfRect.y}, {bottomRightOfRect.x, currentBody.y}, bottomRightOfRect };
				float closestDistance = circCircle.radius;
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

				if (closestDistance < circCircle.radius)
				{
					auto intersectionDistance = circCircle.radius - closestDistance;
					intersectionDistance /= 2.f;

					auto distanceVector = rectCorners[closestCornerIndex] - circleCenter;
					distanceVector /= closestDistance;
					distanceVector *= intersectionDistance;

					currentBody.pos += distanceVector;
					circRect.pos -= distanceVector;
				}
			}
		}

		// circle-on-circle collisions
		for (auto current = kinematicCircObjects.begin(); current != kinematicCircObjects.end(); ++current)
		{
			auto& currentRect = kinematicCircObjects.get<BodyRect>(*current);
			auto& currentCirc = kinematicCircObjects.get<BodyCircle>(*current);
			auto currentCenter = currentRect.pos + currentCirc.offset;

			auto another = current;
			++another;

			for (; another != kinematicCircObjects.end(); ++another)
			{
				auto& anotherRect = kinematicCircObjects.get<BodyRect>(*another);
				auto& anotherCirc = kinematicCircObjects.get<BodyCircle>(*another);
				auto anotherCenter = anotherRect.pos + anotherCirc.offset;

				auto distance = distanceBetweenPoints(currentCenter, anotherCenter);
				float radiusSum = currentCirc.radius + anotherCirc.radius;

				if (distance < radiusSum)
				{
					auto intersectionDistance = radiusSum - distance;
					intersectionDistance /= 2.f;

					auto distanceVector = currentCenter - anotherCenter;
					distanceVector /= distance;
					distanceVector *= intersectionDistance;

					currentRect.pos += distanceVector;
					anotherRect.pos -= distanceVector;
				}
			}
		}
	}
}
