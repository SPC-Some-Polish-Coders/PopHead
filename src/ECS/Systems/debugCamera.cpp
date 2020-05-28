#include "pch.hpp"
#include "debugCamera.hpp"
#include "ECS/Components/graphicsComponents.hpp"
#include "ECS/Components/physicsComponents.hpp"
#include "ECS/Components/charactersComponents.hpp"
#include "Renderer/renderer.hpp"

extern bool debugWindowOpen; 

namespace ph::system {

void DebugCamera::update(float dt)
{
	PH_PROFILE_FUNCTION();

	if(debugWindowOpen && ImGui::BeginTabItem("camera"))
	{
		if(ImGui::Checkbox("debug camera", &mDebugCameraEnabled))
		{
			component::Camera::currentCameraName = "default";

			auto debugCameras = mRegistry.view<component::DebugCamera, component::Camera, component::BodyRect>();
			mRegistry.destroy(debugCameras.begin(), debugCameras.end());

			if(mDebugCameraEnabled)
			{
				// get player pos
				Vec2 playerPos;
				auto players = mRegistry.view<component::Player, component::BodyRect>();
				players.each([&playerPos](const component::Player, const component::BodyRect& body) {
					playerPos = body.center();
				});

				// create debug camera
				auto entity = mRegistry.create();
				mRegistry.assign<component::Camera>(entity, Camera(playerPos, {640, 360}), "debug");
				mRegistry.assign<component::DebugCamera>(entity);
				mRegistry.assign<component::BodyRect>(entity, FloatRect(playerPos, {0.f, 0.f}));
				component::Camera::currentCameraName = "debug";
			}

			sPause = mDebugCameraEnabled;
		}

		if(mDebugCameraEnabled)
		{
			ImGui::Separator();
			ImGui::Text("AWSD - Move camera");

			mRegistry.view<component::DebugCamera, component::Camera, component::BodyRect>().each([this, dt]
			(auto, auto& camera, auto& body)
			{
				if(ImGui::SliderFloat("zoom", &mZoom, 0.01f, 20.f))
				{
					camera.setSize(Vec2(640.f, 360.f) * mZoom);
				}

				ImGui::SliderFloat("movement speed", &mMovementSpeed, 0.01f, 10.f);

				if(ImGui::Button("normal zoom"))
				{
					mZoom = 1.f;
					camera.setSize(Vec2(640.f, 360.f));
				}
			});
		}

		mRegistry.view<component::Camera>().each([&]
		(auto camera)
		{
			if(camera.name == component::Camera::currentCameraName)
			{
				auto center = camera.center();
				auto size = camera.getSize();
				ImGui::Text("camera center: %f %f", center.x, center.y); 
				ImGui::Text("camera size: %f %f", size.x, size.y);
			}
		});

		ImGui::EndTabItem();
	}

	// move camera
	mRegistry.view<component::DebugCamera, component::Camera, component::BodyRect>().each([this, dt]
	(auto, auto& camera, auto& body)
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
		camera.setCenter(body.center());
	});
}

}
