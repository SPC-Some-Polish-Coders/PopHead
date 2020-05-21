#include "pch.hpp"
#include "playerCameraMovement.hpp"
#include "ECS/Components/graphicsComponents.hpp"
#include "ECS/Components/charactersComponents.hpp"
#include "ECS/Components/physicsComponents.hpp"

extern bool debugWindowOpen;

namespace ph::system {

void PlayerCameraMovement::update(float dt)
{
	PH_PROFILE_FUNCTION();

	if(debugWindowOpen && ImGui::BeginTabItem("cam"))
	{
		ImGui::Text("mCurrentCamRoom: %u", mCurrentCamRoom);
		ImGui::Text("mInterpolation: %f", mInterpolation);
		ImGui::EndTabItem();
	}

	if(sPause) return;

	mRoomsThatIntersectPlayer.clear();

	mRegistry.view<component::Player, component::Camera, component::BodyRect, component::FaceDirection>().each([&]
	(auto, auto& playerCam, const auto& playerBody, auto faceDir) 
	{
		mRegistry.view<component::CameraRoom, component::BodyRect>().each([&]
		(auto camRoomEntity, auto& camRoom, const auto& camRoomBody)
		{
			if(intersect(camRoomBody, playerBody))
			{
				camRoom.timeFromPlayerEntrance += dt;
				mRoomsThatIntersectPlayer.emplace_back(camRoomEntity);
			}
			else
			{
				camRoom.timeFromPlayerEntrance = 0.f;
			}
		});

		if(mRoomsThatIntersectPlayer.empty())
		{
			// control camera outside camera room

			mCurrentCamRoom = entt::null;

			if(mInterpolation > 0.f)
			{
				mInterpolation -= dt;
				playerCam.setCenter(Math::lerp(playerCam.center(), playerBody.center(), dt * 5.f));
				playerCam.setSize(Math::lerp(playerCam.getSize(), {640.f, 360.f}, dt * 5.f));
			}

			if(mInterpolation < 0.f)	
				mInterpolation = 0.f;

			if(mInterpolation == 0.f)
			{
				auto newCenter = Math::lerp(playerCam.center(), playerBody.center() + faceDir * 20.f, 6.f * dt);
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
					auto& camRoom = mRegistry.get<component::CameraRoom>(e);
					if(camRoom.timeFromPlayerEntrance < smallestTimeFromPlayerEntrance)
					{
						smallestTimeFromPlayerEntrance = camRoom.timeFromPlayerEntrance;
						mCurrentCamRoom = e;
					}
				}
			}

			if(mCurrentCamRoom != lastCamRoom) 
				mInterpolation = 0.f;

			auto& camRoom = mRegistry.get<component::CameraRoom>(mCurrentCamRoom);
			const auto& camRoomBody = mRegistry.get<component::BodyRect>(mCurrentCamRoom);

			if(mInterpolation < 1.f)
			{
				mInterpolation += dt;
				playerCam.setCenter(Math::lerp(playerCam.center(), camRoomBody.center(), dt * 5.f));
				playerCam.setSize(Math::lerp(playerCam.getSize(), camRoomBody.size, dt * 5.f));
			}

			if(mInterpolation > 1.f) 
				mInterpolation = 1.f;
		}
	});
}

}

