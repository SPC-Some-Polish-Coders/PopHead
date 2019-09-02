#pragma once

#include <string>

namespace ph {

class GameData;
class Xml;

class XmlResourceParser
{
public:
	void parseFile(GameData* const gameData, const std::string& filePath);

private:
	void loadTextures(const Xml& loadingNode, GameData* const gameData);
};

}