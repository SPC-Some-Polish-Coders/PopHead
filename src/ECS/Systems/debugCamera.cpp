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

	auto debugCameraView = mRegistry.view<component::DebugCamera, component::Camera, component::BodyRect>();

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
				sf::Vector2f playerPos;
				auto players = mRegistry.view<component::Player, component::BodyRect>();
				players.each([&playerPos](const component::Player, const component::BodyRect& body) {
					playerPos = body.rect.getCenter();
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

			debugCameraView.each([this, dt](const component::DebugCamera, component::Camera& camera, component::BodyRect& body)
			{
				if(ImGui::SliderFloat("zoom", &mZoom, 0.01f, 20.f))
				{
					camera.camera.setSize(sf::Vector2f(640.f, 360.f) * mZoom);
				}
				
				ImGui::SliderFloat("movement speed", &mMovementSpeed, 0.01f, 10.f);

				if(ImGui::Button("normal zoom"))
				{
					mZoom = 1.f;
					camera.camera.setSize(sf::Vector2f(640.f, 360.f));
				}
			});
		}
		ImGui::EndTabItem();
	}

	// move camera
	debugCameraView.each([this, dt](const component::DebugCamera, component::Camera& camera, component::BodyRect& body)
	{
		sf::Vector2f movement;
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
		body.rect.move(movement);
		camera.camera.setCenter(body.rect.getCenter());
	});
}

}
