#pragma once

#include <string>

namespace ph {

	class GameObjectsParser
	{
		virtual void parseFile(const std::string& fileName) = 0;
	};
}
