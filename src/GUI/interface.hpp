#pragma once

#include "widgetParent.hpp"
#include "widget.hpp"
#include "Events/event.hpp"
#include <vector>

namespace ph {

class Interface : public WidgetParent
{
public:
	Interface(const char* name);

	void handleEvent(const Event&);
	void update(float dt);

	void show();
	void hide();

	Widget* addChildWidget(Widget* widget) override;

	bool isActive() { return mIsActive; }
	const char* getName() { return mName; }

private:
	char mName[50];
	bool mIsActive = true;
};

}

