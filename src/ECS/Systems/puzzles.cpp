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
		mRegistry.view<PressurePlate, PuzzleId>().each([&]
		(auto plate, auto plateId)
		{
			if(plateId.puzzleId == 1)
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

		mRegistry.view<PressurePlate, PuzzleColor, PuzzleId>().each([&]
		(auto plate, auto plateColor, auto plateId)
		{
			if(plateId.puzzleId == 2)
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

		mRegistry.view<PressurePlate, PuzzleId>().each([&]
		(auto plate, auto plateId)
		{
			if(plateId.puzzleId == 3 && plate.isPressed)
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
		mRegistry.view<PressurePlate, PuzzleId>().each([&]
		(auto plate, auto plateId)
		{
			if(plateId.puzzleId == 4) 
			{
				mRegistry.view<Spikes, PuzzleId>().each([&]
				(auto& spikes, auto spikesId)
				{
					if(plateId.hash == spikesId.hash)
						spikes.active = !plate.isPressed;
				});
			}
		});
	}

	// puzzle 5
	{
		mRegistry.view<Lever, PuzzleId>().each([&]
		(auto lever, auto leverId)
		{
			if(leverId.puzzleId == 5 && lever.wasJustSwitched)
			{
				mRegistry.view<Spikes, PuzzleId>().each([&]
				(auto& spikes, auto spikesId)
				{
					if((leverId.elementId == 1 && (spikesId.elementId == 2 || spikesId.elementId == 4)) ||
					   (leverId.elementId == 2 && (spikesId.elementId == 2 || spikesId.elementId == 3 || spikesId.elementId == 4)) ||
					   (leverId.elementId == 3 && (spikesId.elementId == 1 || spikesId.elementId == 2 || spikesId.elementId == 3)) ||
					   (leverId.elementId == 4 && (spikesId.elementId == 2)))
					{
						spikes.active = !spikes.active;
					}
				});
			}
		});
	}

	// puzzle 6
	{
		mRegistry.view<Lever, PuzzleId>().each([&]
		(auto lever, auto leverId)
		{
			if(leverId.puzzleId == 6)
			{
				mRegistry.view<MovingPlatform, PuzzleId>().each([&]
				(auto& platform, auto platformId)
				{
					if(leverId.hash == platformId.hash)
					{
						bool negative = platform.fullVelocity.x < 0.f;
						switch(platformId.elementId)
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

	// puzzle 7 (it's not really a puzzle, it's just lever which opens gate)
	{
		mRegistry.view<Lever, PuzzleId>().each([&]
		(auto lever, auto leverId)
		{
			if(leverId.puzzleId == 7)
			{
				mRegistry.view<Gate>().each([&]
				(auto& gate)
				{
					if(gate.id == 4)
						gate.open = lever.active;
				});
			}
		});
	}

	// puzzle 8
	{
		u32 pressedPlates = 0;

		mRegistry.view<PressurePlate, PuzzleColor, PuzzleId>().each([&]
		(auto plate, auto plateColor, auto plateId)
		{
			if(plateId.puzzleId == 8)
			{
				if(plate.pressedByColor == plateColor)
					++pressedPlates;
			}
		});

		mRegistry.view<Gate>().each([=]
		(auto& gate)
		{
			if(gate.id == 5)
				gate.open = pressedPlates == 2;
		});
	}

	// puzzle 9 (it's not really a puzzle, it's just bunch of pressure plates which open a door)
	{
		u32 pressedPlates = 0;

		mRegistry.view<PressurePlate, PuzzleId>().each([&]
		(auto plate, auto plateId)
		{
			if(plateId.puzzleId == 9 && plate.isPressed)
				++pressedPlates;
		});

		if(pressedPlates == 4)
		{
			mRegistry.view<Gate>().each([&]
			(auto& gate)
			{
				gate.open = true;
			});
		}
	}
}

}
