#pragma once

#include <vector>
#include <memory>

namespace ph {

class Widget;

class WidgetParent
{
public:
	virtual Widget* addChildWidget(Widget* ptr) = 0;
	Widget* getWidget(const char* name);

protected:
	std::vector<std::unique_ptr<Widget>> mWidgetChildren;
};

}
