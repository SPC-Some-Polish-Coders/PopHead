#pragma once

#include <string>

namespace ph {

	class GameData;

	class XmlGuiParser
	{
	public:
		void parseFile(GameData* const gameData, const std::string& fileName);
	};
}
