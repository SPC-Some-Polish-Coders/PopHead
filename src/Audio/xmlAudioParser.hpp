#pragma once

#include <string>

namespace ph {

class GameData;
class Xml;

class XmlAudioParser
{
public:
	void parseFile(GameData* const gameData, const std::string& filePath);

private:
	void parseSoundMute(const Xml& audioNode);
	void parseMusicTheme(const Xml& audioNode);

private:
	GameData* mGameData;
};

}
