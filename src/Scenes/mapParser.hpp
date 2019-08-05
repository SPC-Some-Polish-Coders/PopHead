#pragma once

#include <string>

namespace ph {

	class MapParser
	{
		virtual void parseFile(const std::string& fileName) = 0;
	};
}
