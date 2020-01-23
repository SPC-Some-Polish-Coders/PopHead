#include "interface.hpp"
#include <cstring>

namespace ph {

Interface::Interface(const char* name)
{
	PH_ASSERT_UNEXPECTED_SITUATION(std::strlen(name) < 50, "Interface name length can be max 50 characters long!");
	std::strcpy(mName, name);
}

void Interface::handleEvent(const Event& e)
{
	for(auto& widget : mWidgetChildren)
		if(widget->isActive())
			widget->handleEvent(e);
}

void Interface::update(float dt)
{
	for(auto& widget : mWidgetChildren)
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

Widget* Interface::addChildWidget(Widget* widget)
{
	return mWidgetChildren.emplace_back(widget).get();
}

Widget* Interface::getWidget(const char* name)
{
	for(auto& widget : mWidgetChildren)
		if(std::strcmp(widget->getName(), name) == 0)
			return widget.get();
	return nullptr;
}

}

