#include "renderSystem.hpp"
#include "ECS/Components/physicsComponents.hpp"
#include "Renderer/renderer.hpp"

namespace ph::system {

RenderSystem::RenderSystem(entt::registry& registry, sf::Window& window)
	:System(registry)
	,mWindow(window)
{
}

void RenderSystem::update(float seconds)
{
	//auto view = mRegistry.view<component::Position, component::Size>();
		
	// TODO_ren: Move camera somewhere. To separate system for example.
	/*Camera camera;
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Num8))
		camera.zoom(1.04f);
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Num9))
		camera.zoom(0.96f);
	*/
	// NOTE: beginScene() should be probably where endScene() is
	//Renderer::beginScene(camera);

	/*view.each([this](const component::Position sprite, const component::Size size) {
		Renderer::submitQuad()
	});*/
}

}
