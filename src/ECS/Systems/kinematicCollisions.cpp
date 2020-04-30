#include "pch.hpp"
#include "kinematicCollisions.hpp"
#include "ECS/Components/physicsComponents.hpp"

namespace ph::system {

	void KinematicCollisions::update(float dt)
	{
		PH_PROFILE_FUNCTION();

		if(sPause)
			return;

		auto kinematicObjects = mRegistry.view<component::BodyRect, component::Kinematics, component::KinematicCollisionBody>();

		for (auto current = kinematicObjects.begin(); current != kinematicObjects.end(); ++current)
		{
			auto& currentBody = kinematicObjects.get<component::BodyRect>(*current);
			auto& currentKin = kinematicObjects.get<component::Kinematics>(*current);

			auto another = current;
			++another;
			if (another == kinematicObjects.end())
				break;

			for (; another != kinematicObjects.end(); ++another)
			{
				auto& anotherBody = kinematicObjects.get<component::BodyRect>(*another);
				auto& anotherKin = kinematicObjects.get<component::Kinematics>(*another);

				if (currentBody.rect.doPositiveRectsIntersect(anotherBody.rect))
				{
					sf::FloatRect intersection;
					currentBody.rect.intersects(anotherBody.rect, intersection);

					if (intersection.width < intersection.height)
					{
						auto halfWidth = intersection.width / 2.f;
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
						auto halfHeight = intersection.height / 2.f;
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
		}
	}
}
