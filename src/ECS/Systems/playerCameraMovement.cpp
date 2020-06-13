#include "pch.hpp"
#include "playerCameraMovement.hpp"
#include "ECS/Components/graphicsComponents.hpp"
#include "ECS/Components/charactersComponents.hpp"
#include "ECS/Components/physicsComponents.hpp"

namespace ph::system {

using namespace component;

void PlayerCameraMovement::update(float dt)
{
	PH_PROFILE_FUNCTION();

	if(sPause) return;

	mRoomsThatIntersectPlayer.clear();

	mRegistry.view<Player, Camera, BodyRect, FaceDirection>().each([&]
	(auto, auto& playerCam, const auto& playerBody, auto faceDir) 
	{
		mRegistry.view<CameraRoom, BodyRect>().each([&]
		(auto camRoomEntity, auto& camRoom, auto camRoomBody)
		{
			if(intersect(camRoomBody, playerBody))
			{
				camRoom.timeFromPlayerEntrance += dt;
				mRoomsThatIntersectPlayer.emplace_back(camRoomEntity);
			}
			else
			{
				camRoom.playerWasInCenter = false;
				camRoom.timeFromPlayerEntrance = 0.f;
			}
		});

		if(mRoomsThatIntersectPlayer.empty())
		{
			// control camera outside camera room

			mCurrentCamRoom = entt::null;

			if(mInterpolation > 0.f)
			{
				// control camera just after player left camera room

				mInterpolation -= dt;
				if(mInterpolation < 0.f)	
					mInterpolation = 0.f;

				playerCam.bounds.setSizeWithFixedCenter(lerp(playerCam.bounds.size, {640.f, 360.f}, dt * 5.f));
				playerCam.bounds.setCenter(lerp(playerCam.bounds.center(), playerBody.center() + faceDir * 20.f, dt * 5.f));
			}
			else
			{
				// free camera sliding movement
				Vec2 newCenter = lerp(playerCam.bounds.center(), playerBody.center() + faceDir * 20.f, 5.f * dt);
				playerCam.bounds.setCenter(newCenter);
			}
		}
		else
		{
			// control camera inside camera room

			auto lastCamRoom = mCurrentCamRoom;

			if(mRoomsThatIntersectPlayer.size() == 1)
			{
				mCurrentCamRoom = mRoomsThatIntersectPlayer[0];
			}
			else
			{
				float smallestTimeFromPlayerEntrance = 10000.f;
				for(auto e : mRoomsThatIntersectPlayer)
				{	
					auto& camRoom = mRegistry.get<CameraRoom>(e);
					if(camRoom.timeFromPlayerEntrance < smallestTimeFromPlayerEntrance)
					{
						smallestTimeFromPlayerEntrance = camRoom.timeFromPlayerEntrance;
						mCurrentCamRoom = e;
					}
				}
			}

			if(mCurrentCamRoom != lastCamRoom) 
				mInterpolation = 0.f;

			auto& camRoom = mRegistry.get<CameraRoom>(mCurrentCamRoom);
			const auto& camRoomBody = mRegistry.get<BodyRect>(mCurrentCamRoom);

			if(mInterpolation < 1.f)
			{
				// control camera just after player entered camera room

				mInterpolation += dt;
				if(mInterpolation > 1.f) 
					mInterpolation = 1.f;

				playerCam.bounds.setSizeWithFixedCenter(lerp(playerCam.bounds.size, camRoomBody.size, dt * 5.f));
				playerCam.bounds.setCenter(lerp(playerCam.bounds.center(), camRoomBody.center(), dt * 5.f));
			}
			else
			{
				playerCam.bounds.setCenter(camRoomBody.center());
			}

			if(camRoom.edgeAreaSize && mInterpolation == 1.f)
			{
				// handle camera movement in edge area 

				Vec2 edgeAreaSize = {camRoom.edgeAreaSize * camRoomBody.w, camRoom.edgeAreaSize * camRoomBody.h};

				FloatRect centerArea = camRoomBody;
				centerArea.x += edgeAreaSize.x;
				centerArea.y += edgeAreaSize.y;
				centerArea.w -= edgeAreaSize.x * 2.f;
				centerArea.h -= edgeAreaSize.y * 2.f;

				if(!camRoom.playerWasInCenter && fullyIntersect(centerArea, playerBody))
					camRoom.playerWasInCenter = true;

				if(camRoom.playerWasInCenter)
				{
					bool playerIsInEdgeArea = !centerArea.contains(playerBody.pos) || !centerArea.contains(playerBody.topRight()) || !centerArea.contains(playerBody.bottomLeft()) || !centerArea.contains(playerBody.bottomRight()); 

					if(playerIsInEdgeArea)
					{
						float offset = 0.f;

						FloatRect leftEdgeArea = {camRoomBody.x, camRoomBody.y, edgeAreaSize.x, camRoomBody.h};
						FloatRect rightEdgeArea = {centerArea.right(), camRoomBody.y, edgeAreaSize.x, camRoomBody.h};
						FloatRect topEdgeArea = {camRoomBody.x, camRoomBody.y, camRoomBody.w, edgeAreaSize.y};
						FloatRect bottomEdgeArea = {camRoomBody.x, centerArea.bottom(), camRoomBody.w, edgeAreaSize.y};

						if(intersect(playerBody, leftEdgeArea))
							offset += abs((playerBody.x - centerArea.x) / edgeAreaSize.x);
						else if(intersect(playerBody, rightEdgeArea))
							offset += abs((playerBody.right() - centerArea.right()) / edgeAreaSize.x); 

						if(intersect(playerBody, topEdgeArea))
							offset += abs((playerBody.y - centerArea.y) / edgeAreaSize.y);
						else if(intersect(playerBody, bottomEdgeArea))
							offset += abs((playerBody.bottom() - centerArea.bottom()) / edgeAreaSize.y);

						playerCam.bounds.setCenter(lerp(camRoomBody.center(), playerBody.center(), offset / 2.2f));
					}
				}
			}
		}
	});
}

}

