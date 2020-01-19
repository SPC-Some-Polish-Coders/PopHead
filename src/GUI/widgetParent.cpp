#include "widgetParent.hpp"
#include "widget.hpp"
#include <cstring>

namespace ph {

Widget* WidgetParent::getWidget(const char* name)
{
	for(auto& widget : mWidgetChildren)
		if(std::strcmp(widget->getName(), name))
			return widget.get();
	return nullptr;
}

}