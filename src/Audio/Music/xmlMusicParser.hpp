#pragma once

#include <string>

namespace ph {

class GameData;

class XmlMusicParser
{
public:
	void parseFile(GameData* const gameData, const std::string& filePath);
};

}