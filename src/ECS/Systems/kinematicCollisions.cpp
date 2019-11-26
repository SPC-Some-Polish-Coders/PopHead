#include "kinematicCollisions.hpp"
#include "ECS/Components/physicsComponents.hpp"

namespace ph::system {

	void KinematicCollisions::update(float seconds)
	{
		auto kinematicObjects = mRegistry.view<component::BodyRect, component::Velocity, component::KinematicCollisionBody>();

		for (auto current = kinematicObjects.begin(); current != kinematicObjects.end(); ++current)
		{
			auto& currentBody = kinematicObjects.get<component::BodyRect>(*current);
			auto& currentVel = kinematicObjects.get<component::Velocity>(*current);

			auto another = current;
			++another;
			if (another == kinematicObjects.end())
				break;

			for (; another != kinematicObjects.end(); ++another)
			{
				auto& anotherBody = kinematicObjects.get<component::BodyRect>(*another);
				auto& anotherVel = kinematicObjects.get<component::Velocity>(*another);

				if (currentBody.rect.doPositiveRectsIntersect(anotherBody.rect))
				{
					component::Velocity newVel = { currentVel.dx + anotherVel.dx, currentVel.dy + anotherVel.dy };

					sf::FloatRect intersection;
					currentBody.rect.intersects(anotherBody.rect, intersection);

					if (intersection.width < intersection.height)
					{
						currentVel.dx = newVel.dx;
						anotherVel.dx = newVel.dx;
						auto halfWidth = intersection.width / 2.f;
						if (currentBody.rect.left < anotherBody.rect.left)
						{
							currentBody.rect.left -= halfWidth;
							anotherBody.rect.left += halfWidth;
						}
						else
						{
							currentBody.rect.left += halfWidth;
							anotherBody.rect.left -= halfWidth;
						}
					}
					else
					{
						currentVel.dy = newVel.dy;
						anotherVel.dy = newVel.dy;
						auto halfHeight = intersection.height / 2.f;
						if (currentBody.rect.top < anotherBody.rect.top)
						{
							currentBody.rect.top -= halfHeight;
							anotherBody.rect.top += halfHeight;
						}
						else
						{
							currentBody.rect.top += halfHeight;
							anotherBody.rect.top -= halfHeight;
						}
					}
				}
			}
		}
	}
}
