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

	mRegistry.view<Player, component::Camera, BodyRect, FaceDirection>().each([&]
	(auto, auto& playerCam, const auto& playerBody, auto faceDir) 
	{
		mRegistry.view<CameraRoom, BodyRect>().each([&]
		(auto camRoomEntity, auto& camRoom, const auto& camRoomBody)
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

				playerCam.setCenter(Math::lerp(playerCam.center(), playerBody.center(), dt * 5.f));
				playerCam.setSize(Math::lerp(playerCam.getSize(), {640.f, 360.f}, dt * 5.f));
			}

			if(mInterpolation == 0.f)
			{
				// free camera sliding movement
				auto newCenter = Math::lerp(playerCam.center(), playerBody.center() + faceDir * 20.f, 5.f * dt);
				playerCam.setCenter(newCenter);
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

				playerCam.setCenter(Math::lerp(playerCam.center(), camRoomBody.center(), dt * 5.f));
				playerCam.setSize(Math::lerp(playerCam.getSize(), camRoomBody.size, dt * 5.f));
			}
			else
			{
				playerCam.setCenter(camRoomBody.center());
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

						auto addOffset = [&offset](float x) 
						{
							x = abs(x);
							if(offset) x /= 2.f;
							offset += x; 
						};

						FloatRect leftEdgeArea = {camRoomBody.x, camRoomBody.y, edgeAreaSize.x, camRoomBody.h};
						if(intersect(playerBody, leftEdgeArea))
							addOffset((playerBody.x - centerArea.x) / edgeAreaSize.x);

						FloatRect rightEdgeArea = {centerArea.right(), camRoomBody.y, edgeAreaSize.x, camRoomBody.h};
						if(intersect(playerBody, rightEdgeArea))
							addOffset((playerBody.right() - centerArea.right()) / edgeAreaSize.x); 

						FloatRect topEdgeArea = {camRoomBody.x, camRoomBody.y, camRoomBody.w, edgeAreaSize.y};
						if(intersect(playerBody, topEdgeArea))
							addOffset((playerBody.y - centerArea.y) / edgeAreaSize.y);

						FloatRect bottomEdgeArea = {camRoomBody.x, centerArea.bottom(), camRoomBody.w, edgeAreaSize.y};
						if(intersect(playerBody, bottomEdgeArea))
							addOffset((playerBody.bottom() - centerArea.bottom()) / edgeAreaSize.y);

						playerCam.setCenter(Math::lerp(camRoomBody.center(), playerBody.center(), offset / 2.f));
					}
				}
			}
		}
	});
}

}

