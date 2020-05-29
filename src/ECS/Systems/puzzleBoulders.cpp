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

	mRegistry.view<component::PuzzleBoulder, component::PuzzleGridPos, component::BodyRect>().each([&]
	(auto& boulder, auto& boulderGridPos, auto& boulderBody)
	{
		mRegistry.view<component::Player, component::Kinematics, component::BodyRect, component::BodyCircle>().each([&]
		(auto, const auto& playerKinematics, const auto& playerBody, auto playerCircle)
		{
			if(playerKinematics.acceleration == Vec2())
				mTimeSincePlayerIsPushingBoulder = 0.f;

			auto playerMovesBoulder = [&](float& pushedSince, float& moving)
			{
				mTimeSincePlayerIsPushingBoulder += dt;
				pushedSince += dt;

				float pushDelay = 0.2f;
				if(mTimeSincePlayerIsPushingBoulder > 0.3f) pushDelay = 0.f;
				else if(mTimeSincePlayerIsPushingBoulder > 0.1f) pushDelay = 0.05f;

				if(pushedSince > pushDelay)
				{
					pushedSince = 0.f;
					moving = 0.0001f;
					boulder.movedGridPosInThisMove = false;
				}
			};

			auto collidesOnPuzzleGrid = [&](Vec2i pushDir)
			{
				auto newBoulderGridPos = boulderGridPos + pushDir;

				// check collsion with another boulder
				auto puzzleBoulders = mRegistry.view<component::PuzzleBoulder, component::PuzzleGridPos>();
				for(auto otherBoulder : puzzleBoulders)
				{
					const auto& otherBoulderGridPos = puzzleBoulders.get<component::PuzzleGridPos>(otherBoulder);
					if(newBoulderGridPos == otherBoulderGridPos)
						return true;
				}

				// check is boulder not going out of the boulder road
				Vec2i gridPosOfRoadChunkThatBoulderIsIn;

				if(newBoulderGridPos.x > 0)
					gridPosOfRoadChunkThatBoulderIsIn.x = newBoulderGridPos.x - newBoulderGridPos.x % 12;
				else if(newBoulderGridPos.x < 0)
					gridPosOfRoadChunkThatBoulderIsIn.x = Cast<i32>(std::floor(Cast<float>(newBoulderGridPos.x) / 12.f)) * 12;

				if(newBoulderGridPos.y > 0)
					gridPosOfRoadChunkThatBoulderIsIn.y = newBoulderGridPos.y - newBoulderGridPos.y % 12;
				else if(newBoulderGridPos.y < 0)
					gridPosOfRoadChunkThatBoulderIsIn.y = Cast<i32>(std::floor(Cast<float>(newBoulderGridPos.y) / 12.f)) * 12;

				Vec2i chunkRelativeBoulderPos = newBoulderGridPos - gridPosOfRoadChunkThatBoulderIsIn;

				auto roadChunks = mRegistry.view<component::PuzzleGridRoadChunk, component::PuzzleGridPos>();
				for(auto roadChunkEntity : roadChunks)
				{
					const auto& roadChunk = roadChunks.get<component::PuzzleGridRoadChunk>(roadChunkEntity);
					const auto& roadChunkGridPos = roadChunks.get<component::PuzzleGridPos>(roadChunkEntity);

					if(gridPosOfRoadChunkThatBoulderIsIn == roadChunkGridPos)
						return !roadChunk.tiles[chunkRelativeBoulderPos.y][chunkRelativeBoulderPos.x];
				}

				return true;
			};

			auto circlePos = playerBody.pos + playerCircle.offset;

			// push boulder left
			auto boulderLeftEdge = boulderBody;
			boulderLeftEdge.x -= 2.f;
			boulderLeftEdge.y += 2.f;
			boulderLeftEdge.w = 5.f;
			boulderLeftEdge.h -= 4.f;
			if(Math::intersect(boulderLeftEdge, circlePos, playerCircle.radius) &&
			   playerKinematics.acceleration.x > 0.f &&
			   (boulder.movingRight == 0.f || boulder.movingRight > 15.f) &&
			   !collidesOnPuzzleGrid({1, 0}))
			{
				playerMovesBoulder(boulder.pushedRightSince, boulder.movingRight);
			}
			else
			{
				boulder.pushedRightSince = 0.f;
			}

			// push boulder rigth
			auto boulderRightEdge = boulderBody;
			boulderRightEdge.x += boulderBody.w - 3.f;
			boulderRightEdge.y += 2.f;
			boulderRightEdge.w = 5.f;
			boulderRightEdge.h -= 4.f;
			if(Math::intersect(boulderRightEdge, circlePos, playerCircle.radius) && 
			   playerKinematics.acceleration.x < 0.f &&
			   (boulder.movingLeft == 0.f || boulder.movingLeft > 15.f) &&
			   !collidesOnPuzzleGrid({-1, 0}))
			{
				playerMovesBoulder(boulder.pushedLeftSince, boulder.movingLeft);
			}
			else
			{
				boulder.pushedLeftSince = 0.f;
			}

			// push boulder up
			auto boulderUpEdge = boulderBody;
			boulderUpEdge.x += 2.f; 
			boulderUpEdge.y -= 2.f; 
			boulderUpEdge.w -= 4.f; 
			boulderUpEdge.h = 5.f;
			if(Math::intersect(boulderUpEdge, circlePos, playerCircle.radius) && 
			   playerKinematics.acceleration.y > 0.f &&
			   (boulder.movingDown == 0.f || boulder.movingDown > 15.f) &&
			   !collidesOnPuzzleGrid({0, 1}))
			{
				playerMovesBoulder(boulder.pushedDownSince, boulder.movingDown);
			}
			else
			{
				boulder.pushedDownSince = 0.f;
			}

			// push boulder down
			auto boulderDownEdge = boulderBody;
			boulderDownEdge.x += 2.f;
			boulderDownEdge.y += boulderBody.h - 3.f;
			boulderDownEdge.w -= 4.f;
			boulderDownEdge.h = 5.f;
			if(Math::intersect(boulderDownEdge, circlePos, playerCircle.radius) &&
			   playerKinematics.acceleration.y < 0.f &&
			   (boulder.movingUp == 0.f || boulder.movingUp > 15.f) &&
			   !collidesOnPuzzleGrid({0, -1}))
			{
				playerMovesBoulder(boulder.pushedUpSince, boulder.movingUp);
			}
			else
			{
				boulder.pushedUpSince = 0.f;
			}

			#if 0 // boulder edges debug visualization
			Renderer::submitQuad(0, 0, &sf::Color(255, 0, 0, 200), 0, boulderLeftEdge.pos, boulderLeftEdge.size, 0, 0.f, {});
			Renderer::submitQuad(0, 0, &sf::Color(0, 255, 0, 200), 0, boulderRightEdge.pos, boulderRightEdge.size, 0, 0.f, {});
			Renderer::submitQuad(0, 0, &sf::Color(0, 0, 200, 200), 0, boulderUpEdge.pos, boulderUpEdge.size, 0, 0.f, {});
			Renderer::submitQuad(0, 0, &sf::Color(255, 255, 0, 200), 0, boulderDownEdge.pos, boulderDownEdge.size, 0, 0.f, {});
			#endif
		});

		float movementSpeed = 48.f * dt;

		if(boulder.movingLeft)
		{
			boulder.movingLeft += movementSpeed;
			boulderBody.x -= movementSpeed;

			if(boulder.movingLeft > 4.f && !boulder.movedGridPosInThisMove)
			{
				boulderGridPos.x -= 1;
				boulder.movedGridPosInThisMove = true;
			}
			
			if(boulder.movingLeft > 16.f)
			{
				boulder.movingLeft = 0.f;
				boulderBody.x = round(boulderBody.x / 16.f) * 16.f;
			}
		}
		else if(boulder.movingRight)
		{
			boulder.movingRight += movementSpeed;
			boulderBody.x += movementSpeed;

			if(boulder.movingRight > 4.f && !boulder.movedGridPosInThisMove)
			{
				boulderGridPos.x += 1;
				boulder.movedGridPosInThisMove = true;
			}

			if(boulder.movingRight > 16.f)
			{
				boulder.movingRight = 0.f;
				boulderBody.x = round(boulderBody.x / 16.f) * 16.f;
			}
		}
		else if(boulder.movingUp)
		{
			boulder.movingUp += movementSpeed;
			boulderBody.y -= movementSpeed;

			if(boulder.movingUp > 4.f && !boulder.movedGridPosInThisMove)
			{
				boulderGridPos.y -= 1;
				boulder.movedGridPosInThisMove = true;
			}

			if(boulder.movingUp > 16.f)
			{
				boulder.movingUp = 0.f;
				boulderBody.y = round(boulderBody.y / 16.f) * 16.f;
			}
		}
		else if(boulder.movingDown)
		{
			boulder.movingDown += movementSpeed;
			boulderBody.y += movementSpeed;

			if(boulder.movingDown > 4.f && !boulder.movedGridPosInThisMove)
			{
				boulderGridPos.y += 1;
				boulder.movedGridPosInThisMove = true;
			}

			if(boulder.movingDown > 16.f)
			{
				boulder.movingDown = 0.f;
				boulderBody.y = round(boulderBody.y / 16.f) * 16.f;
			}
		}
	});
}

}

