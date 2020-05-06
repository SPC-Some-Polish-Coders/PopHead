#include "pch.hpp"
#include "puzzles.hpp"
#include "ECS/Components/objectsComponents.hpp"

namespace ph::system {

void Puzzles::update(float dt)
{
	if(sPause)
		return;

	auto pressurePlates = mRegistry.view<component::PressurePlate>();
	auto gates = mRegistry.view<component::Gate>();

	mRegistry.view<component::Puzzle>().each([&]
	(component::Puzzle& puzzle)
	{
		switch(puzzle.id)
		{
			case 1:
			{
				pressurePlates.each([&]
				(component::PressurePlate plate)
				{
					if(plate.puzzleId == puzzle.id && plate.isPressed)
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
		}
	});
}

}
