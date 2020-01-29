#include "debugCamera.hpp"
#include "ECS/Components/graphicsComponents.hpp"
#include "ECS/Components/physicsComponents.hpp"
#include "Renderer/renderer.hpp"
#include <SFML/Window/Keyboard.hpp>

namespace ph::system {

void DebugCamera::onEvent(Event event)
{
	if(auto* e = std::get_if<sf::Event>(&event))
		if(e->type == sf::Event::KeyPressed && e->key.code == sf::Keyboard::H)
			mIsHintActive = !mIsHintActive;
}

void DebugCamera::update(float dt)
{
	auto view = mRegistry.view<component::DebugCamera, component::Camera, component::BodyRect>();
	view.each([this, dt](const component::DebugCamera, component::Camera& camera, component::BodyRect& body)
	{
		if(mIsHintActive)
		{
			float posY = 10.f;
			auto drawDebugCameraHintText = [&posY](const char* text, bool bold = false) {
				float size = bold ? 50.f : 30.f;
				Renderer::submitText(text, bold ? "LiberationMono-Bold.ttf" : "LiberationMono.ttf", {10.f, posY}, size, sf::Color::White, 0, ProjectionType::gui);
				posY += bold ? 75.f : 30.f;
			};
			drawDebugCameraHintText("DebugCamera mode", true); 
			drawDebugCameraHintText("Ctrl + Tab - Back to terminal");
			drawDebugCameraHintText("AWSD - Movement");
			drawDebugCameraHintText("K - Slow movement");
			drawDebugCameraHintText("K + L - Very slow movement");
			drawDebugCameraHintText("J - Fast movement");
			drawDebugCameraHintText("J + L - Very fast movement");
			drawDebugCameraHintText("Arrow keys - Zooming");
			drawDebugCameraHintText("H - Hide/Show hint");
			Renderer::submitQuad(nullptr, nullptr, &sf::Color(0, 0, 0, 150), nullptr, {}, {650.f, 350.f}, 1, 0.f, {}, ProjectionType::gui); 
		}

		// move camera
		sf::Vector2f movement;
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
			movement.x -= 500.f;	
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
			movement.x += 500.f;	
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
			movement.y -= 500.f;	
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
			movement.y += 500.f;	
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::K)) { 
			movement *= 0.4f;
			if(sf::Keyboard::isKeyPressed(sf::Keyboard::L))
				movement *= 0.5f;
		}
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::J)) { 
			movement *= 2.f;
			if(sf::Keyboard::isKeyPressed(sf::Keyboard::L))
				movement *= 3.f;
		}

		movement *= dt;
		body.rect.move(movement);
		camera.camera.setCenterSmoothly(body.rect.getCenter(), 10 * dt);

		// zoom camera 
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
			camera.camera.zoom(1.01f);
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
			camera.camera.zoom(0.99f);
	});
}

}
