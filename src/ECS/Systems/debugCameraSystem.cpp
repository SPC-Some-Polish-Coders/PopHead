#include "pch.hpp"
#include "debugCameraSystem.hpp"
#include "ECS/Components/graphicsComponents.hpp"
#include "ECS/Components/physicsComponents.hpp"
#include "ECS/Components/charactersComponents.hpp"
#include "ECS/entityUtil.hpp"
#include "Renderer/renderer.hpp"

extern bool debugWindowOpen; 

namespace ph::system {

using namespace component;

void DebugCameraSystem::update(float dt)
{
	PH_PROFILE_FUNCTION();

	if(debugWindowOpen && ImGui::BeginTabItem("camera"))
	{
		if(ImGui::Checkbox("debug camera", &mDebugCameraEnabled))
		{
			auto debugCameras = mRegistry.view<DebugCamera, Camera, BodyRect>();
			mRegistry.destroy(debugCameras.begin(), debugCameras.end());

			if(mDebugCameraEnabled)
			{
				// create debug camera
				Vec2 camCenterPos = isPlayerOnScene() ? getPlayerCenterPos() : Vec2();
				auto entity = mRegistry.create();
				mRegistry.assign<Camera>(entity, Camera{"debug", FloatRect(camCenterPos - Vec2(320.f, 180.f), Vec2(640.f, 360.f))});
				mRegistry.assign<DebugCamera>(entity);
				mRegistry.assign<BodyRect>(entity, FloatRect(camCenterPos, {0.f, 0.f}));
				Camera::currentCameraName = "debug";
			}
			else
			{
				Camera::currentCameraName = "default";
			}

			sPause = mDebugCameraEnabled;
		}

		if(mDebugCameraEnabled)
		{
			ImGui::Checkbox("debug camera moves", &mDebugCameraMoves);

			ImGui::Separator();
			
			if(mDebugCameraMoves)
				ImGui::Text("AWSD - Move camera");

			mRegistry.view<DebugCamera, Camera>().each([this, dt]
			(auto, auto& camera)
			{
				if(ImGui::SliderFloat("zoom", &mZoom, 0.01f, 20.f))
					camera.bounds.size = Vec2(640.f, 360.f) * mZoom;

				if(mDebugCameraMoves)
					ImGui::SliderFloat("movement speed", &mMovementSpeed, 0.01f, 10.f);

				if(ImGui::Button("normal zoom"))
				{
					mZoom = 1.f;
					camera.bounds.size = Vec2(640.f, 360.f);
				}
			});
		}

		mRegistry.view<Camera>().each([&]
		(auto camera)
		{
			if(camera.name == Camera::currentCameraName)
			{
				auto center = camera.bounds.center();
				auto size = camera.bounds.size;
				ImGui::Text("camera center: %f %f", center.x, center.y); 
				ImGui::Text("camera size: %f %f", size.x, size.y);
			}
		});

		ImGui::EndTabItem();
	}

	// move camera
	mRegistry.view<DebugCamera, Camera, BodyRect>().each([this, dt]
	(auto, auto& camera, auto& body)
	{
		if(mDebugCameraMoves)
		{
			Vec2 movement;
			if(sf::Keyboard::isKeyPressed(sf::Keyboard::A)) 
				movement.x -= 500.f;	
			if(sf::Keyboard::isKeyPressed(sf::Keyboard::D))
				movement.x += 500.f;	
			if(sf::Keyboard::isKeyPressed(sf::Keyboard::W))
				movement.y -= 500.f;	
			if(sf::Keyboard::isKeyPressed(sf::Keyboard::S))
				movement.y += 500.f;	
			movement *= dt;
			movement *= mMovementSpeed;
			body.pos += movement;
		}
		camera.bounds.setCenter(body.pos);
	});
}

}
