#include "xmlAudioParser.hpp"
#include "Utilities/xml.hpp"
#include "Logs/logs.hpp"
#include "gameData.hpp"

namespace ph {

void XmlAudioParser::parseFile(GameData* const gameData, const std::string& filePath)
{
	PH_LOG_INFO("Music file (" + filePath + ") is being parsed.");

	Xml musicFile;
	musicFile.loadFromFile(filePath);
	const Xml musicNode = musicFile.getChild("music");
	const Xml startThemeNode = musicNode.getChild("starttheme");
	const std::string themeFilePath = "music/" + startThemeNode.getAttribute("filename").toString();
	gameData->getMusicPlayer().play(themeFilePath);
}

}
