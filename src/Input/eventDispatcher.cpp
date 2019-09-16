#include "eventDispatcher.hpp"

namespace ph {

bool EventDispatcher::dispatchEvent(ph::Event& event, sf::Window& window)
{
	sf::Event sfmlEvent;
	if(window.pollEvent(sfmlEvent)) {
		event = sfmlEvent;
		return true;
	}
	return false;
	
}

}