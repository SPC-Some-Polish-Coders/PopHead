#include "eventDispatcher.hpp"

namespace ph {

bool EventDispatcher::dispatchEvent(ph::Event& event, sf::Window& window)
{
	sf::Event sfmlEvent;
	if(window.pollEvent(sfmlEvent)) {
		event = sfmlEvent;
		ActionEventManager::addActionEventsTo(mPendingActionEvents, sfmlEvent);
		return true;
	}

	if(!mPendingActionEvents.empty()) {
		event = mPendingActionEvents.back();
		mPendingActionEvents.pop_back();
		return true;
	}

	return false;
}

}