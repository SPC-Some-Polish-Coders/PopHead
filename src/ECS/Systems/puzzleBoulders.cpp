#include "pch.hpp"
#include "puzzleBoulders.hpp"
#include "Renderer/renderer.hpp"
#include "ECS/Components/objectsComponents.hpp"
#include "ECS/Components/physicsComponents.hpp"
#include "ECS/Components/charactersComponents.hpp"

namespace ph::system {

void PuzzleBoulders::update(float dt)
{
	PH_PROFILE_FUNCTION();

	if(sPause)
		return;

	mRegistry.view<component::PuzzleBoulder, component::BodyRect>().each([&]
	(auto& boulder, auto& boulderBody)
	{
		mRegistry.view<component::Player, component::BodyRect>().each([&]
		(auto, const auto& playerBody)
		{
			auto playerMovesBoulder = [=](float& pushedSince, bool& isMoving)
			{
				pushedSince += dt;
				if(pushedSince > 0.3f)
				{
					pushedSince = 0.f;
					isMoving = true;
				}
			};

			auto boulderLeftEdge = boulderBody;
			boulderLeftEdge.x -= 2.f;
			boulderLeftEdge.w = 5.f;
			if(intersect(boulderLeftEdge, playerBody))
			{
				playerMovesBoulder(boulder.pushedRightSince, boulder.isMovingRight);
			}
			else
			{
				boulder.pushedRightSince = 0.f;
			}

			auto boulderRightEdge = boulderBody;
			boulderRightEdge.x += boulderBody.w - 3.f;
			boulderRightEdge.w = 5.f;
			Renderer::submitQuad(0, 0, &sf::Color(255, 0, 0, 200), 0, boulderRightEdge.pos, boulderRightEdge.size, 0, 0.f, {});
			if(intersect(boulderRightEdge, playerBody))
			{
				playerMovesBoulder(boulder.pushedLeftSince, boulder.isMovingLeft);
			}
			else
			{
				boulder.pushedLeftSince = 0.f;
			}

			auto boulderUpEdge = boulderBody;
			boulderUpEdge.y -= 2.f; 
			boulderUpEdge.h = 5.f;
			if(intersect(boulderUpEdge, playerBody))
			{
				playerMovesBoulder(boulder.pushedDownSince, boulder.isMovingDown);
			}
			else
			{
				boulder.pushedDownSince = 0.f;
			}

			auto boulderDownEdge = boulderBody;
			boulderDownEdge.y += boulderBody.h - 3.f;
			boulderDownEdge.h = 5.f;
			if(intersect(boulderDownEdge, playerBody))
			{
				playerMovesBoulder(boulder.pushedUpSince, boulder.isMovingUp);
			}
			else
			{
				boulder.pushedUpSince = 0.f;
			}
		});

		if(boulder.isMovingLeft)
		{
			boulderBody.x -= 16.f;
			boulder.isMovingLeft = false;
		}
		else if(boulder.isMovingRight)
		{
			boulderBody.x += 16.f;
			boulder.isMovingRight = false;
		}
		else if(boulder.isMovingUp)
		{
			boulderBody.y -= 16.f;
			boulder.isMovingUp = false;
		}
		else if(boulder.isMovingDown)
		{
			boulderBody.y += 16.f;
			boulder.isMovingDown = false;
		}

	});
}

}

