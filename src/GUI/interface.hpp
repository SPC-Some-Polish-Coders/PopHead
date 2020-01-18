#pragma once

#include <vector>
#include "widget.hpp"
#include "Events/event.hpp"

namespace ph {

class Interface
{
public:
	Interface(const char* name);

	void handleEvent(const Event&);
	void update(float dt);

	void show();
	void hide();

	void addWidget(const char* name, Widget* widget);
	Widget* getWidget(const char* name);

	bool isActive() { return mIsActive; }
	const char* getName() { return mName; }

private:
	char mName[50];
	std::vector<std::unique_ptr<Widget>> mWidgets;
	bool mIsActive = true;
};

}

