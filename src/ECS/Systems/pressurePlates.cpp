#include "pch.hpp"
#include "pressurePlates.hpp"
#include "ECS/Components/objectsComponents.hpp"
#include "ECS/Components/physicsComponents.hpp"
#include "ECS/Components/graphicsComponents.hpp"

namespace ph::system {

void PressurePlates::update(float dt)
{
	if(sPause)
		return;

	auto kinematicBodies = mRegistry.view<component::KinematicCollisionBody, component::BodyRect>(); 

	mRegistry.view<component::PressurePlate, component::BodyRect, component::TextureRect>().each([&]
	(component::PressurePlate& plate, const component::BodyRect& plateBody, component::TextureRect& textureRect)
	{
		plate.isPressed = false;

		kinematicBodies.each([&]
		(const component::KinematicCollisionBody, const component::BodyRect& kinBody)	
		{
			if(intersect(plateBody, kinBody))
			{
				plate.isPressed = true;
				return;
			}
		});

		if(plate.isPressed)
			textureRect.y = 18;
		else
			textureRect.y = 0;
	});
}

}
