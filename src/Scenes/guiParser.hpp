#pragma once

#include <string>

namespace ph {

	class GUIParser
	{
		virtual void parseFile(const std::string& fileName) = 0;
	};
}
