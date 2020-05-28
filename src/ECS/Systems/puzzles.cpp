#include "pch.hpp"
#include "puzzles.hpp"
#include "ECS/Components/objectsComponents.hpp"
#include "ECS/Components/physicsComponents.hpp"

namespace ph::system {

Puzzles::Puzzles(entt::registry& registry, EntitiesTemplateStorage& ets)
:System(registry)
,mEntitiesTemplateStorage(ets)
{
}

void Puzzles::update(float dt)
{
	PH_PROFILE_FUNCTION();

	if(sPause) return;

	// puzzle 1	
	{
		mRegistry.view<component::PressurePlate>().each([&]
		(auto plate)
		{
			if(plate.puzzleId == 1)
			{
				mRegistry.view<component::Gate>().each([=]
				(auto& gate)
				{
					if(gate.id == 1)
						gate.open = plate.isPressed;
				});	
			}
		});
	}

	// puzzle 2
	{
		u32 pressedPlates = 0;

		mRegistry.view<component::PressurePlate, component::PuzzleColor>().each([&]
		(auto plate, auto plateColor)
		{
			if(plate.puzzleId == 2)
			{
				if(plate.pressedByColor == plateColor)
					++pressedPlates;
			}
		});

		mRegistry.view<component::Gate>().each([=]
		(auto& gate)
		{
			if(gate.id == 2)
				gate.open = pressedPlates == 3;
		});
	}

	// puzzle 3
	{
		u32 pressedPlates = 0;

		mRegistry.view<component::PressurePlate>().each([&]
		(auto plate)
		{
			if(plate.puzzleId == 3 && plate.isPressed)
				++pressedPlates;
		});

		mRegistry.view<component::Gate>().each([=]
		(auto& gate)
		{
			if(gate.id == 3)
				gate.open = pressedPlates == 4;
		});
	}

	// puzzle 4
	{
		mRegistry.view<component::PressurePlate>().each([&]
		(auto plate)
		{
			if(plate.puzzleId == 4) 
			{
				mRegistry.view<component::Spikes>().each([&]
				(auto& spikes)
				{
					if(spikes.puzzleId == 4 && plate.id == spikes.id)
						spikes.active = !plate.isPressed;
				});
			}
		});
	}
}

}
