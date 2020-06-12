#include "pch.hpp"
#include "puzzles.hpp"
#include "ECS/Components/objectsComponents.hpp"
#include "ECS/Components/physicsComponents.hpp"

namespace ph::system {

using namespace component;

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
		mRegistry.view<PressurePlate>().each([&]
		(auto plate)
		{
			if(plate.puzzleId == 1)
			{
				mRegistry.view<Gate>().each([=]
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

		mRegistry.view<PressurePlate, PuzzleColor>().each([&]
		(auto plate, auto plateColor)
		{
			if(plate.puzzleId == 2)
			{
				if(plate.pressedByColor == plateColor)
					++pressedPlates;
			}
		});

		mRegistry.view<Gate>().each([=]
		(auto& gate)
		{
			if(gate.id == 2)
				gate.open = pressedPlates == 3;
		});
	}

	// puzzle 3
	{
		u32 pressedPlates = 0;

		mRegistry.view<PressurePlate>().each([&]
		(auto plate)
		{
			if(plate.puzzleId == 3 && plate.isPressed)
				++pressedPlates;
		});

		mRegistry.view<Gate>().each([=]
		(auto& gate)
		{
			if(gate.id == 3)
				gate.open = pressedPlates == 4;
		});
	}

	// puzzle 4
	{
		mRegistry.view<PressurePlate>().each([&]
		(auto plate)
		{
			if(plate.puzzleId == 4) 
			{
				mRegistry.view<Spikes>().each([&]
				(auto& spikes)
				{
					if(spikes.puzzleId == 4 && plate.id == spikes.id)
						spikes.active = !plate.isPressed;
				});
			}
		});
	}

	// puzzle 5
	{
		mRegistry.view<Lever>().each([&]
		(auto lever)
		{
			if(lever.puzzleId == 5 && lever.wasJustSwitched)
			{
				mRegistry.view<Spikes>().each([&]
				(auto& spikes)
				{
					if((lever.id == 1 && (spikes.id == 2 || spikes.id == 4)) ||
					   (lever.id == 2 && (spikes.id == 2 || spikes.id == 3 || spikes.id == 4)) ||
					   (lever.id == 3 && (spikes.id == 1 || spikes.id == 2 || spikes.id == 3)) ||
					   (lever.id == 4 && (spikes.id == 2)))
					{
						spikes.active = !spikes.active;
					}
				});
			}
		});
	}

	// puzzle 6
	{
		mRegistry.view<Lever>().each([&]
		(auto lever)
		{
			if(lever.puzzleId == 6)
			{
				mRegistry.view<MovingPlatform>().each([&]
				(auto& platform)
				{
					if(lever.id == platform.id)
					{
						bool negative = platform.fullVelocity.x < 0.f;
						switch(platform.id)
						{
							case 1: platform.fullVelocity.x = lever.active ? 45.f : 60.f; break;
							case 2: platform.fullVelocity.x = lever.active ? 45.f : 90.f; break;
							case 3: platform.fullVelocity.x = lever.active ? 45.f : 120.f; break;
							case 4: platform.fullVelocity.x = lever.active ? 45.f : 150.f; break;
							case 5: platform.fullVelocity.x = lever.active ? 45.f : 200.f; break;
						}
						if(negative)
							platform.fullVelocity.x = -platform.fullVelocity.x;
					}
				});
			}
		});
	}
}

}
