#pragma once

#include <string>
#include <memory>
#include <entt/entt.hpp>
#include <SFML/Window/Window.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/Color.hpp>

namespace ph {
	class SceneManager;

	struct OutputLine
	{
		std::string text;
		sf::Color color;
	};
}

namespace ph::Terminal
{
	void init(sf::Window*, SceneManager*);
	void handleEvent(sf::Event);
	void update(float dt);
	void pushOutputLine(const OutputLine&);
}
