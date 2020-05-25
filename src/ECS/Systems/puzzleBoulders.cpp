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
		mRegistry.view<component::Player, component::Kinematics, component::BodyRect>().each([&]
		(auto, const auto& playerKinematics, const auto& playerBody)
		{
			if(playerKinematics.acceleration == sf::Vector2f())
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

			auto collidesOnPuzzleGrid = [&](sf::Vector2i pushDir)
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
				sf::Vector2i gridPosOfRoadChunkThatBoulderIsIn;

				if(newBoulderGridPos.x > 0)
					gridPosOfRoadChunkThatBoulderIsIn.x = newBoulderGridPos.x - newBoulderGridPos.x % 12;
				else if(newBoulderGridPos.x < 0)
					gridPosOfRoadChunkThatBoulderIsIn.x = static_cast<int>(std::floor(static_cast<float>(newBoulderGridPos.x) / 12.f)) * 12;

				if(newBoulderGridPos.y > 0)
					gridPosOfRoadChunkThatBoulderIsIn.y = newBoulderGridPos.y - newBoulderGridPos.y % 12;
				else if(newBoulderGridPos.y < 0)
					gridPosOfRoadChunkThatBoulderIsIn.y = static_cast<int>(std::floor(static_cast<float>(newBoulderGridPos.y) / 12.f)) * 12;

				sf::Vector2i chunkRelativeBoulderPos = newBoulderGridPos - gridPosOfRoadChunkThatBoulderIsIn;

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

			// push boulder right
			auto boulderLeftEdge = boulderBody;
			boulderLeftEdge.x -= 2.f;
			boulderLeftEdge.w = 5.f;
			if(intersect(boulderLeftEdge, playerBody) && playerKinematics.acceleration.x > 0.f &&
			   (boulder.movingRight == 0.f || boulder.movingRight > 15.f) &&
			   !collidesOnPuzzleGrid({1, 0}))
			{
				playerMovesBoulder(boulder.pushedRightSince, boulder.movingRight);
			}
			else
			{
				boulder.pushedRightSince = 0.f;
			}

			// push boulder left
			auto boulderRightEdge = boulderBody;
			boulderRightEdge.x += boulderBody.w - 3.f;
			boulderRightEdge.w = 5.f;
			if(intersect(boulderRightEdge, playerBody) && playerKinematics.acceleration.x < 0.f &&
			   (boulder.movingLeft == 0.f || boulder.movingLeft > 15.f) &&
			   !collidesOnPuzzleGrid({-1, 0}))
			{
				playerMovesBoulder(boulder.pushedLeftSince, boulder.movingLeft);
			}
			else
			{
				boulder.pushedLeftSince = 0.f;
			}

			// push boulder down
			auto boulderUpEdge = boulderBody;
			boulderUpEdge.y -= 2.f; 
			boulderUpEdge.h = 5.f;
			if(intersect(boulderUpEdge, playerBody) && playerKinematics.acceleration.y > 0.f &&
			   (boulder.movingDown == 0.f || boulder.movingDown > 15.f) &&
			   !collidesOnPuzzleGrid({0, 1}))
			{
				playerMovesBoulder(boulder.pushedDownSince, boulder.movingDown);
			}
			else
			{
				boulder.pushedDownSince = 0.f;
			}

			// push boulder up
			auto boulderDownEdge = boulderBody;
			boulderDownEdge.y += boulderBody.h - 3.f;
			boulderDownEdge.h = 5.f;
			if(intersect(boulderDownEdge, playerBody) && playerKinematics.acceleration.y < 0.f &&
			   (boulder.movingUp == 0.f || boulder.movingUp > 15.f) &&
			   !collidesOnPuzzleGrid({0, -1}))
			{
				playerMovesBoulder(boulder.pushedUpSince, boulder.movingUp);
			}
			else
			{
				boulder.pushedUpSince = 0.f;
			}
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

