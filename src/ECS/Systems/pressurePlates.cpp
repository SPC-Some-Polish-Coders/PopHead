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

	mRegistry.view<component::PressurePlate, component::PuzzleColor, component::BodyRect, component::TextureRect>().each([&]
	(component::PressurePlate& plate, component::PuzzleColor plateColor,
	 const component::BodyRect& plateBody, component::TextureRect& textureRect)
	{
		if(plate.isPressIrreversible && plate.isPressed)
			return;

		plate.isPressed = false;

		kinematicBodies.each([&]
		(auto pressedByEntity, const component::KinematicCollisionBody, const component::BodyRect& kinBody)	
		{
			if(intersect(plateBody, kinBody))
			{
				plate.isPressed = true;

				using component::PuzzleColor;
				if(plateColor != PuzzleColor::Grey)
				{
					if(const auto* pressedByColor = mRegistry.try_get<component::PuzzleColor>(pressedByEntity))
						plate.pressedByColor = *pressedByColor;
					else if(!plate.isPressed)
						plate.pressedByColor = PuzzleColor::Grey;
				}
			}
		});

		if(plate.isPressed)
			textureRect.y = 18;
		else
			textureRect.y = 0;
	});
}

}
