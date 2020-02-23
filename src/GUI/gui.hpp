#pragma once

#include "interface.hpp"
#include "widget.hpp"
#include "textWidget.hpp"
#include "sliderWidget.hpp"
#include <SFML/Window/Event.hpp>

namespace ph::GUI
{
	void init();
	void shutDown();

	void handleEvent(sf::Event);
	void update(float dt);

	Interface* addInterface(const char* name);
	Interface* getInterface(const char* name);
	bool hasInterface(const char* name);
	void deleteInterface(const char* name);
	void showInterface(const char* name);
	void hideInterface(const char* name);
	void clear();
}

