#pragma once

#include "ECS/system.hpp"

#include <SFML/Graphics/RenderWindow.hpp>

namespace ph::system {

	class Renderer : public System
	{
	public:
		Renderer(entt::registry& registry, sf::RenderWindow& window);

		void update(float seconds) override;

	private:
		sf::RenderWindow& mRenderWindow;
	};
}
