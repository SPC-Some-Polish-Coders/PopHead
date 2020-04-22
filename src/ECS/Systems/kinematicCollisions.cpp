#include "kinematicCollisions.hpp"
#include "ECS/Components/physicsComponents.hpp"
#include "Utilities/profiling.hpp"

namespace ph::system {

	void KinematicCollisions::update(float dt)
	{
		PH_PROFILE_FUNCTION();

		if (sPause)
			return;

		auto kinematicRectObjects = mRegistry.view<component::KinematicCollisionBody, component::Velocity, component::BodyRect>();
		auto kinematicCircObjects = mRegistry.view<component::KinematicCollisionBody, component::Velocity, component::BodyCircle>();

		for (auto current = kinematicRectObjects.begin(); current != kinematicRectObjects.end(); ++current)
		{
			auto& currentBody = kinematicRectObjects.get<component::BodyRect>(*current);
			auto& currentVel = kinematicRectObjects.get<component::Velocity>(*current);

			auto another = current;
			++another;
			if (another == kinematicRectObjects.end())
				break;

			// rect-on-rect collisions
			for (; another != kinematicRectObjects.end(); ++another)
			{
				auto& anotherBody = kinematicRectObjects.get<component::BodyRect>(*another);
				//auto& anotherVel = kinematicRectObjects.get<component::Velocity>(*another);

				if (currentBody.rect.doPositiveRectsIntersect(anotherBody.rect))
				{
					//component::Velocity newVel = { currentVel.dx + anotherVel.dx, currentVel.dy + anotherVel.dy };

					sf::FloatRect intersection;
					currentBody.rect.intersects(anotherBody.rect, intersection);

					if (intersection.width < intersection.height)
					{
						//currentVel.dx = newVel.dx;
						//anotherVel.dx = newVel.dx;
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
						//currentVel.dy = newVel.dy;
						//anotherVel.dy = newVel.dy;
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
			// rect-on-circle collisions
			for (auto anotherC : kinematicCircObjects)
			{
				auto& anotherBody = kinematicCircObjects.get<component::BodyCircle>(anotherC);
				//auto& anotherVel = kinematicCircObjects.get<component::Velocity>(anotherC);

				auto nx = std::fmaxf(currentBody.rect.left, std::fminf(anotherBody.center.x, currentBody.rect.right()));
				auto ny = std::fmaxf(currentBody.rect.top, std::fminf(anotherBody.center.y, currentBody.rect.bottom()));
				auto dx = anotherBody.center.x - nx;
				auto dy = anotherBody.center.y - ny;
				auto dd = dx * dx + dy * dy;
				if (dd < anotherBody.radius * anotherBody.radius)
				{
					auto bb = sf::FloatRect(sf::Vector2f(anotherBody.center.x - anotherBody.radius, anotherBody.center.y - anotherBody.radius), sf::Vector2f(anotherBody.radius * 2, anotherBody.radius * 2));
					sf::FloatRect intersection;
					currentBody.rect.intersects(bb, intersection);
					if (intersection.width < intersection.height)
					{
						auto halfWidth = intersection.width / 2.f;
						if (currentBody.rect.left < anotherBody.center.x - anotherBody.radius)
						{
							currentBody.rect.left -= halfWidth;
							anotherBody.center.x += halfWidth;
						}
						else
						{
							currentBody.rect.left += halfWidth;
							anotherBody.center.x -= halfWidth;
						}
					}
					else
					{
						auto halfHeight = intersection.width / 2.f;
						if (currentBody.rect.top < anotherBody.center.x + anotherBody.radius)
						{
							currentBody.rect.top -= halfHeight;
							anotherBody.center.y += halfHeight;
						}
						else
						{
							currentBody.rect.top += halfHeight;
							anotherBody.center.y -= halfHeight;
						}
					}
				}
			}
		}

		// circle-on-circle collisions
		for (auto current = kinematicCircObjects.begin(); current != kinematicCircObjects.end(); ++current)
		{
			auto& currentBody = kinematicCircObjects.get<component::BodyCircle>(*current);

			auto another = current;
			++another;
			if (another == kinematicCircObjects.end())
				break;

			for (; another != kinematicCircObjects.end(); ++another)
			{
				auto& anotherBody = kinematicCircObjects.get<component::BodyCircle>(*another);
				auto dx = anotherBody.center.x - currentBody.center.x;
				auto dy = anotherBody.center.y - currentBody.center.y;
				if (dx * dx + dy * dy < (anotherBody.radius + currentBody.radius) * (anotherBody.radius + currentBody.radius))
				{
					if (dx * dx < dy * dy) 
					{
						auto hdx = dx / 2.;
						currentBody.center.x -= hdx;
						anotherBody.center.x += hdx;
					}
					else {
						auto hdy = dy / 2.;
						currentBody.center.y -= hdy;
						anotherBody.center.y += hdy;
					}
				}
			}
		}

	}
}
