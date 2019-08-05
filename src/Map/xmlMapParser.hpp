#pragma once

#include <string>

namespace ph {

	class GameData;

	class XmlMapParser
	{
	public:
		void parseFile(GameData* const gameData, const std::string& fileName);
	};
}
