#include "pch.hpp"
#include "puzzles.hpp"
#include "ECS/Components/objectsComponents.hpp"

namespace ph::system {

void Puzzles::update(float dt)
{
	if(sPause)
		return;

	auto gates = mRegistry.view<component::Gate>();

	mRegistry.view<component::Puzzle>().each([&]
	(component::Puzzle& puzzle)
	{
		switch(puzzle.id)
		{
			case 1:
			{
				mRegistry.view<component::PressurePlate>().each([&]
				(component::PressurePlate plate)
				{
					if(plate.puzzleId == puzzle.id)
					{
						gates.each([=]
						(component::Gate& gate)
						{
							if(gate.id == 1)
								gate.open = plate.isPressed;
						});	
					}
				});

			} break;

			case 2:
			{
				unsigned pressedPlates = 0;

				mRegistry.view<component::PressurePlate, component::PuzzleColor>().each([&]
				(component::PressurePlate plate, component::PuzzleColor plateColor)
				{
					if(plate.puzzleId == puzzle.id)
					{
						if(plate.pressedByColor == plateColor)
							++pressedPlates;
					}
				});

				gates.each([=]
				(component::Gate& gate)
				{
					if(gate.id == 2)
						gate.open = pressedPlates == 3;
				});
			} break;
		}
	});
}

}
