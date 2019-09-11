#pragma once

#include <string>

namespace ph {

class GameData;

class XmlAudioParser
{
public:
	void parseFile(GameData* const gameData, const std::string& filePath);
};

}
