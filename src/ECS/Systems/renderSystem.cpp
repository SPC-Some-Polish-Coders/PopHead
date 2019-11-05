#include "renderSystem.hpp"
#include "ECS/Components/physicsComponents.hpp"
#include "ECS/Components/graphicsComponents.hpp"
#include "Renderer/renderer.hpp"

namespace ph::system {

RenderSystem::RenderSystem(entt::registry& registry, sf::Window& window)
	:System(registry)
	,mCamera()
	,mWindow(window)
{
}

void RenderSystem::update(float seconds)
{
	auto view = mRegistry.view<component::Position, component::Size, component::TextureRef>();
		
	// TODO_ren: Move camera somewhere. To separate system for example.
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Num8))
		mCamera.zoom(1.04f);
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Num9))
		mCamera.zoom(0.96f);

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		mCamera.move({-1.f, 0.f});
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		mCamera.move({1.f, 0.f});
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		mCamera.move({0.f, -1.f});
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		mCamera.move({0.f, 1.f});
	
	// NOTE: beginScene() should be probably where endScene() is
	Renderer::beginScene(mCamera);
	Texture texture("resources/textures/vehicles/car.png");
	Renderer::submitQuad(texture, {10.f, 10.f}, texture.getSize());

	view.each([this](const component::Position position, const component::Size size, const component::TextureRef textureRef) {
		Renderer::submitQuad(*textureRef.texture, {position.x, position.y}, {(int)size.width, (int)size.height});
	});
}

}
