#pragma once

#include <string>

namespace ph {

	class GameData;
	class GameObject;

	class XmlGameObjectsParser
	{
	public:
		void parseFile(GameData* const gameData, GameObject& root, const std::string& fileName);
	};
}
