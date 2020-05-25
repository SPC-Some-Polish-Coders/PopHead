#include "pch.hpp"
#include "pressurePlates.hpp"
#include "ECS/Components/objectsComponents.hpp"
#include "ECS/Components/physicsComponents.hpp"
#include "ECS/Components/graphicsComponents.hpp"

namespace ph::system {

void PressurePlates::update(float dt)
{
	PH_PROFILE_FUNCTION();

	if(sPause)
		return;

	mRegistry.view<component::PressurePlate, component::PuzzleColor, component::PuzzleGridPos,
	               component::BodyRect, component::TextureRect>().each([&]
	(auto& plate, auto plateColor, auto pressurePlatePuzzleGridPos, const auto& plateBody, auto& textureRect)
	{
		if(plate.isPressIrreversible && plate.isPressed)
			return;

		plate.isPressed = false;

		auto pressPlate = [&](entt::entity pressingEntity)
		{
			plate.isPressed = true;

			using component::PuzzleColor;
			if(plateColor != PuzzleColor::Grey)
			{
				if(const auto* pressedByColor = mRegistry.try_get<component::PuzzleColor>(pressingEntity))
					plate.pressedByColor = *pressedByColor;
				else if(!plate.isPressed)
					plate.pressedByColor = PuzzleColor::Grey;
			}
		};

		mRegistry.view<component::KinematicCollisionBody, component::BodyRect>().each([&]
		(auto pressingEntity, auto, const auto& kinBody)	
		{
			if(intersect(plateBody, kinBody))
				pressPlate(pressingEntity);
		});

		mRegistry.view<component::PuzzleBoulder, component::PuzzleGridPos>().each([&]
		(auto pressingEntity, auto, auto boulderPuzzleGridPos)
		{
			if(boulderPuzzleGridPos == pressurePlatePuzzleGridPos)
				pressPlate(pressingEntity);
		});

		if(plate.isPressed)
			textureRect.y = 18;
		else
			textureRect.y = 0;
	});
}

}
