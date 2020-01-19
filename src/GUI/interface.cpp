#include "interface.hpp"
#include <cstring>

namespace ph {

Interface::Interface(const char* name)
{
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

}

