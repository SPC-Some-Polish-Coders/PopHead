#include "pch.hpp"
#include "pressurePlates.hpp"
#include "ECS/Components/objectsComponents.hpp"
#include "ECS/Components/physicsComponents.hpp"

namespace ph::system {

void PressurePlates::update(float dt)
{
	if(sPause)
		return;

	auto kinematicBodies = mRegistry.view<component::KinematicCollisionBody, component::BodyRect>(); 

	mRegistry.view<component::PressurePlate, component::BodyRect>().each([&]
	(component::PressurePlate& plate, const component::BodyRect& plateBody)
	{
		plate.isPressed = false;

		kinematicBodies.each([&]
		(const component::KinematicCollisionBody, const component::BodyRect& kinBody)	
		{
			if(intersect(plateBody, kinBody))
			{
				plate.isPressed = true;
			}
		});
	});
}

}
