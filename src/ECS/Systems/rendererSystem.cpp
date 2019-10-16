#include "rendererSystem.hpp"
#include "ECS/Components/graphicsComponents.hpp"

namespace ph::system {

	Renderer::Renderer(entt::registry& registry, sf::RenderWindow& window)
		: System(registry)
		, mRenderWindow(window)
	{
	}

	void Renderer::update(float seconds)
	{
		auto view = mRegistry.view<component::Sprite>();
		
		mRenderWindow.clear();
		view.each([this](const component::Sprite& sprite) {
			mRenderWindow.draw(sprite.sprite);
			});
		mRenderWindow.display();
	}
}
