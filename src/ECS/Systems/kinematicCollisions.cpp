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

		for(auto current = kinematicObjects.begin(); current != kinematicObjects.end(); ++current)
		{
			auto& currentBody = kinematicObjects.get<component::BodyRect>(*current);
			auto& currentKin = kinematicObjects.get<component::Kinematics>(*current);

			auto another = current;
			++another;
			if(another == kinematicObjects.end())
				break;

			for(; another != kinematicObjects.end(); ++another)
			{
				auto& anotherBody = kinematicObjects.get<component::BodyRect>(*another);
				auto& anotherKin = kinematicObjects.get<component::Kinematics>(*another);

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
