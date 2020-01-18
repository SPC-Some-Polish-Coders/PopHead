#include "interface.hpp"
#include <cstring>

namespace ph {

Interface::Interface(const char* name)
{
	std::strcpy(mName, name);
}

void Interface::handleEvent(const Event& e)
{
	for(auto& widget : mWidgets)
		if(widget->isActive())
			widget->handleEvent(e);
}

void Interface::update(float dt)
{
	for(auto& widget : mWidgets)
		if(widget->isActive())
			widget->update(dt, 20);
}

void Interface::show()
{
	mIsActive = true;
}

void Interface::hide()
{
	mIsActive = false;
}

void Interface::addWidget(Widget* widget)
{
	mWidgets.emplace_back(widget);
}

Widget* Interface::getWidget(const char* name)
{
	for(auto& widget : mWidgets)
		if(std::strcmp(widget->getName(), name) == 0)
			return widget.get();
	return nullptr;
}

}

