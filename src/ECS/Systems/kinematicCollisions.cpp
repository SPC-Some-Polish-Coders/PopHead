#include "pch.hpp"
#include "kinematicCollisions.hpp"
#include "ECS/Components/physicsComponents.hpp"

namespace ph::system {

	void KinematicCollisions::update(float dt)
	{
		PH_PROFILE_FUNCTION();

		if (sPause)
			return;

		auto kinematicRectObjects = mRegistry.view<component::KinematicCollisionBody, component::Kinematics, component::BodyRect>();
		auto kinematicCircObjects = mRegistry.view<component::KinematicCollisionBody, component::Kinematics, component::BodyCircle>();

		for (auto current = kinematicRectObjects.begin(); current != kinematicRectObjects.end(); ++current)
		{
			auto& currentBody = kinematicRectObjects.get<component::BodyRect>(*current);
			auto& currentKin = kinematicRectObjects.get<component::Kinematics>(*current);

			auto another = current;
			++another;

			if (another == kinematicRectObjects.end())
				break;

			// rect-on-rect collisions
			for (; another != kinematicRectObjects.end(); ++another)
			{
				auto& anotherBody = kinematicRectObjects.get<component::BodyRect>(*another);

				if(intersect(currentBody, anotherBody))
				{
					FloatRect intersection;
					currentBody.intersects(anotherBody, intersection);

					if(intersection.w < intersection.h)
					{
						auto halfWidth = intersection.w / 2.f;
						if(currentBody.x < anotherBody.x)
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
						if(currentBody.y < anotherBody.y)
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
