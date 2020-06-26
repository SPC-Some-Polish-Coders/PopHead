#include "pch.hpp"
#include "puzzles.hpp"
#include "ECS/Components/objectsComponents.hpp"
#include "ECS/Components/physicsComponents.hpp"
#include "ECS/Components/simRegionComponents.hpp"

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

	mRegistry.view<Puzzle, InsideSimRegion>().each([&]
	(auto puzzle, auto)
	{
		switch(puzzle.id)
		{
			case 1:
			{
				mRegistry.view<PressurePlate, PuzzleId>().each([&]
				(auto plate, auto plateId)
				{
					if(plateId.puzzleId == puzzle.id)
					{
						mRegistry.view<Gate>().each([=]
						(auto& gate)
						{
							if(gate.id == 1)
								gate.open = plate.isPressed;
						});	
					}
				});
			} break;

			case 2:
			{
				u32 pressedPlates = 0;

				mRegistry.view<PressurePlate, PuzzleColor, PuzzleId>().each([&]
				(auto plate, auto plateColor, auto plateId)
				{
					if(plateId.puzzleId == puzzle.id)
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
			} break;

			case 3:
			{
				u32 pressedPlates = 0;

				mRegistry.view<PressurePlate, PuzzleId>().each([&]
				(auto plate, auto plateId)
				{
					if(plateId.puzzleId == puzzle.id && plate.isPressed)
						++pressedPlates;
				});

				mRegistry.view<Gate>().each([=]
				(auto& gate)
				{
					if(gate.id == 3)
						gate.open = pressedPlates == 4;
				});
			} break;

			case 4:
			{
				mRegistry.view<PressurePlate, PuzzleId>().each([&]
				(auto plate, auto plateId)
				{
					if(plateId.puzzleId == puzzle.id) 
					{
						mRegistry.view<Spikes, PuzzleId>().each([&]
						(auto& spikes, auto spikesId)
						{
							if(plateId.hash == spikesId.hash)
								spikes.active = !plate.isPressed;
						});
					}
				});
			} break;

			case 5:
			{
				mRegistry.view<Lever, PuzzleId>().each([&]
				(auto lever, auto leverId)
				{
					if(leverId.puzzleId == puzzle.id && lever.wasJustSwitched)
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
			} break;

			case 6:
			{
				mRegistry.view<Lever, PuzzleId>().each([&]
				(auto lever, auto leverId)
				{
					if(leverId.puzzleId == puzzle.id)
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
			} break;

			case 7:
			{
				// it's not really a puzzle, it's just lever which opens gate

				mRegistry.view<Lever, PuzzleId>().each([&]
				(auto lever, auto leverId)
				{
					if(leverId.puzzleId == puzzle.id)
					{
						mRegistry.view<Gate>().each([&]
						(auto& gate)
						{
							if(gate.id == 4)
								gate.open = lever.active;
						});
					}
				});
			} break;

			case 8:
			{
				u32 pressedPlates = 0;

				mRegistry.view<PressurePlate, PuzzleColor, PuzzleId>().each([&]
				(auto plate, auto plateColor, auto plateId)
				{
					if(plateId.puzzleId == puzzle.id)
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
			} break;

			case 9:
			{
				// it's not really a puzzle, it's just bunch of pressure plates which open a door

				u32 pressedPlates = 0;

				mRegistry.view<PressurePlate, PuzzleId>().each([&]
				(auto plate, auto plateId)
				{
					if(plateId.puzzleId == puzzle.id && plate.isPressed)
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
			} break;

			default: PH_BREAKPOINT();
		}
	});
}

}
