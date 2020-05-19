#include "pch.hpp"
#include "playerCameraMovement.hpp"
#include "ECS/Components/graphicsComponents.hpp"
#include "ECS/Components/charactersComponents.hpp"
#include "ECS/Components/physicsComponents.hpp"

namespace ph::system {

void PlayerCameraMovement::update(float dt)
{
	PH_PROFILE_FUNCTION();

	if(sPause) return;

	mRegistry.view<component::Player, component::Camera, component::BodyRect, component::FaceDirection>().each([&]
	(auto, auto& playerCam, const auto& playerBody, auto faceDir) 
	{
		bool isPlayerInCamRoom = false;
		float lastPlayerCamRoomCenterDistance;

		mRegistry.view<component::CameraRoom, component::BodyRect>().each([&]
		(auto& camRoom, const auto& camRoomBody)
		{
			if(intersect(camRoomBody, playerBody))
			{
				float playerCamRoomCenterDistance = Math::distanceBetweenPoints(camRoomBody.center(), playerBody.center());  
				if(isPlayerInCamRoom)
				{
					if(playerCamRoomCenterDistance < lastPlayerCamRoomCenterDistance)
						lastPlayerCamRoomCenterDistance = playerCamRoomCenterDistance; 
					else
						return;
				}
				else
				{
					lastPlayerCamRoomCenterDistance = playerCamRoomCenterDistance;
					isPlayerInCamRoom = true;
				}

				if(camRoom.interpolation < 1.f)
				{
					camRoom.interpolation += dt;
					playerCam.setCenter(Math::lerp(playerCam.center(), camRoomBody.center(), dt * 5.f));
					playerCam.setSize(Math::lerp(playerCam.getSize(), camRoomBody.size, dt * 5.f));
				}

				if(camRoom.interpolation > 1.f) 
					camRoom.interpolation = 1.f;
			}
			else
			{
				if(camRoom.interpolation > 0.f)
				{
					camRoom.interpolation -= dt;
					playerCam.setCenter(Math::lerp(playerCam.center(), playerBody.center(), dt * 5.f));
					playerCam.setSize(Math::lerp(playerCam.getSize(), {640.f, 360.f}, dt * 5.f));
				}

				if(camRoom.interpolation < 0.f)	
					camRoom.interpolation = 0.f;
			}
		});

		if(!isPlayerInCamRoom)
		{
			auto newCenter = Math::lerp(playerCam.center(), playerBody.center() + faceDir * 20.f, 6.f * dt);
			playerCam.setCenter(newCenter);
		}
	});
}

}

