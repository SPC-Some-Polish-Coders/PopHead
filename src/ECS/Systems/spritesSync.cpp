#include "spritesSync.hpp"
#include "ECS/Components/graphicsComponents.hpp"
#include "ECS/Components/physicsComponents.hpp"

namespace ph::system {

	void SpritesSync::update(float seconds)
	{
		auto view = mRegistry.view<component::Position, component::Sprite>();

		view.each([](const component::Position& pos, component::Sprite& sprite) {
			sprite.sprite.setPosition(pos.x, pos.y);
			});
	}
}
