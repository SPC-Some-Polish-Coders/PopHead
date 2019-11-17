#pragma once

#include <vector>
#include <SFML/Graphics.hpp>

namespace ph {

// This is only temporary stuff and it's used only for drawing GUI
// Later we'll draw everything including GUI with our own renderer

class SFMLRenderer
{
public:
	void submit(const sf::Drawable*);

	unsigned getNumberOfSubmitedObjects() const;

	void flush(sf::RenderWindow& window);

private:
	std::vector<const sf::Drawable*> mSubmitedObjects;
};

}
