#include "gui.hpp"
#include <cstring>

namespace ph {

Interface* GUI::addInterface(const char* name)
{
	return &mInterfaces.emplace_back(name);
}

Interface* GUI::getInterface(const char* name)
{
	for(auto& interface : mInterfaces)
		if(strcmp(interface.getName(), name) == 0)
			return &interface;
	return nullptr;
}

bool GUI::hasInterface(const char* name)
{
	return getInterface(name);
}

void GUI::deleteInterface(const char* name)
{
	for(auto it = mInterfaces.begin(); it != mInterfaces.end(); ++it)
		if(strcmp(it->getName(), name) == 0)
			mInterfaces.erase(it);
}

void GUI::showInterface(const char* name)
{
	if(auto* interface = getInterface(name))
		interface->show();
}

void GUI::hideInterface(const char* name)
{
	if(auto* interface = getInterface(name))
		interface->hide();
}

void GUI::handleEvent(const ph::Event& e)
{
	for(auto& interface : mInterfaces)
		interface.handleEvent(e);
}

void GUI::update(float dt)
{
	for(auto& interface : mInterfaces)
		if(interface.isActive())
			interface.update(dt);
}

void GUI::clearGUI()
{
	mInterfaces.clear();
}

}
