#pragma once

#include <vector>
#include <SFML/Graphics.hpp>

namespace ph {

// This is only temporary stuff and it's used only for drawing GUI
// Later we'll draw everything including GUI with our own renderer

class SFMLRenderer
{
public:
	void submit(sf::Drawable*);

	void drawSubmitedObjects(sf::RenderWindow& window);

private:
	std::vector<sf::Drawable*> mSubmitedObjects;
};

}
