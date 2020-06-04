#include "pch.hpp"
#include "pressurePlates.hpp"
#include "ECS/Components/objectsComponents.hpp"
#include "ECS/Components/physicsComponents.hpp"
#include "ECS/Components/graphicsComponents.hpp"

namespace ph::system {

using namespace component;

void PressurePlates::update(float dt)
{
	PH_PROFILE_FUNCTION();

	if(sPause) return;

	mRegistry.view<PressurePlate, PuzzleColor, PuzzleGridPos, BodyRect, TextureRect>().each([&]
	(auto& plate, auto plateColor, auto pressurePlatePuzzleGridPos, auto plateBody, auto& textureRect)
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
				if(const auto* pressedByColor = mRegistry.try_get<PuzzleColor>(pressingEntity))
					plate.pressedByColor = *pressedByColor;
				else if(!plate.isPressed)
					plate.pressedByColor = PuzzleColor::Grey;
			}
		};

		mRegistry.view<KinematicCollisionBody, BodyRect, BodyCircle>().each([&]
		(auto pressingEntity, auto, auto kinBody, auto kinCircle)
		{
			if(intersect(plateBody, kinBody, kinCircle))
				pressPlate(pressingEntity);
		});

		mRegistry.view<PuzzleBoulder, PuzzleGridPos>().each([&]
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
