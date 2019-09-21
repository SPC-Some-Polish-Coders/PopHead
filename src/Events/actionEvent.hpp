#pragma once

#include <string>

namespace ph {

struct ActionEvent
{
	enum Type { Pressed, Released };

	std::string mAction;
	Type mType;

	ActionEvent(const std::string action, const Type type);
};

}
