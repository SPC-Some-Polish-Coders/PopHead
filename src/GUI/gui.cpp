#include "pch.hpp"
#include "gui.hpp"

namespace ph::GUI {

namespace {
	std::vector<Interface> mInterfaces;
}

void init()
{
	Widget::initShader();
}

void shutDown()
{
	Widget::deleteShader();
}

Interface* addInterface(const char* name)
{
	return &mInterfaces.emplace_back(name);
}

Interface* getInterface(const char* name)
{
	for(auto& interface : mInterfaces)
		if(std::strcmp(interface.getName(), name) == 0)
			return &interface;
	return Null;
}

bool hasInterface(const char* name)
{
	return getInterface(name) != Null;
}

void deleteInterface(const char* name)
{
	for(auto it = mInterfaces.begin(); it != mInterfaces.end(); ++it)
		if(std::strcmp(it->getName(), name) == 0)
			mInterfaces.erase(it);
}

void showInterface(const char* name)
{
	if(auto* interface = getInterface(name))
		interface->show();
}

void hideInterface(const char* name)
{
	if(auto* interface = getInterface(name))
		interface->hide();
}

void handleEvent(sf::Event e)
{
	if(e.type == sf::Event::Resized)
		Widget::setScreenSize(Vec2(Cast<float>(e.size.width), Cast<float>(e.size.height)));

	for(auto& interface : mInterfaces)
		if(interface.isActive())
			interface.handleEvent(e);
}

void update(float dt)
{
	for(auto& interface : mInterfaces)
		if(interface.isActive())
			interface.update(dt);
}

void clear()
{
	mInterfaces.clear();
}

}

