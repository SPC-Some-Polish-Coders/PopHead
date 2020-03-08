#include "debugCamera.hpp"
#include "ECS/Components/graphicsComponents.hpp"
#include "ECS/Components/physicsComponents.hpp"
#include "Utilities/math.hpp"
#include "Utilities/profiling.hpp"
#include "Renderer/renderer.hpp"
#include <SFML/Window/Keyboard.hpp>

namespace ph::system {

void DebugCamera::onEvent(sf::Event e)
{
	if(e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::H)
		mIsHintActive = !mIsHintActive;
}

void DebugCamera::update(float dt)
{
	PH_PROFILE_FUNCTION(0);

	auto view = mRegistry.view<component::DebugCamera, component::Camera, component::BodyRect>();
	view.each([this, dt](const component::DebugCamera, component::Camera& camera, component::BodyRect& body)
	{
		// show hint
		if(mIsHintActive)
		{
			float posY = 10.f;
			auto drawDebugCameraHintText = [&posY](const char* text, bool bold = false) {
				float size = bold ? 50.f : 30.f;
				Renderer::submitText(text, bold ? "LiberationMono-Bold.ttf" : "LiberationMono.ttf", {10.f, posY}, size,
					sf::Color::White, 0, ProjectionType::gui);
				posY += bold ? 75.f : 30.f;
			};
			drawDebugCameraHintText("DebugCamera mode", true); 
			drawDebugCameraHintText("Ctrl + Tab - Back to terminal");
			drawDebugCameraHintText("H - Hide/Show hint");
			drawDebugCameraHintText("AWSD - Movement");
			drawDebugCameraHintText("Arrow keys - Zooming");
			drawDebugCameraHintText("K - Slow movement");
			drawDebugCameraHintText("K + L - Very slow movement");
			drawDebugCameraHintText("J - Fast movement");
			drawDebugCameraHintText("J + L - Very fast movement");
			Renderer::submitQuad(nullptr, nullptr, &sf::Color(0, 0, 0, 150), nullptr, {}, {650.f, 350.f},
				1, 0.f, {}, ProjectionType::gui, false); 
		}

		// get modifier flags 
		bool speedUp = sf::Keyboard::isKeyPressed(sf::Keyboard::J);
		bool slowDown = sf::Keyboard::isKeyPressed(sf::Keyboard::K);
		bool magnification = sf::Keyboard::isKeyPressed(sf::Keyboard::L);

		// move camera
		sf::Vector2f movement;
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::A)) 
			movement.x -= 500.f;	
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::D))
			movement.x += 500.f;	
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::W))
			movement.y -= 500.f;	
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::S))
			movement.y += 500.f;	
		if(slowDown && magnification)
			movement *= 0.1f;
		else if(slowDown) 
			movement *= 0.4f;
		else if(speedUp && magnification)
			movement *= 6.f;
		else if(speedUp)
			movement *= 2.f;
		movement *= dt;
		body.rect.move(movement);
		camera.camera.setCenter(body.rect.getCenter());

		// zoom camera 
		float zoom = 1.f;
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
			zoom = 1.01f;
			if(slowDown && magnification)
				zoom = 1.003f;
			else if(slowDown)
				zoom = 1.005f;
			else if(speedUp && magnification)
				zoom = 1.04f;
			else if(speedUp)
				zoom = 1.02f;
		}	
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
			zoom = 0.99f;
			if(slowDown && magnification)
				zoom = 0.996f; 
			else if(slowDown)
				zoom = 0.993f;
			else if(speedUp && magnification)
				zoom = 0.98f;
			else if(speedUp)
				zoom = 0.96f;
		}
		camera.camera.zoom(zoom);
	});
}

}
