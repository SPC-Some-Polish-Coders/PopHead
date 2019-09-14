#include "xmlAudioParser.hpp"
#include "Utilities/xml.hpp"
#include "Logs/logs.hpp"
#include "gameData.hpp"

namespace ph {

void XmlAudioParser::parseFile(GameData* const gameData, const std::string& filePath)
{
	PH_LOG_INFO("Music file (" + filePath + ") is being parsed.");

	mGameData = gameData;

	Xml audioFile;
	audioFile.loadFromFile(filePath);
	const Xml audioNode = audioFile.getChild("audio");
	parseVolume(audioNode);
	parseMusicTheme(audioNode);
}

void XmlAudioParser::parseVolume(const Xml& audioNode)
{
	const std::string defaultVolume = "default";
	const Xml volumeNode = audioNode.getChild("volume");

	std::string soundVolume = volumeNode.getAttribute("soundvolume").toString();
	if(soundVolume == defaultVolume)
		mGameData->getSoundPlayer().setVolume(mGameData->getSoundPlayer().getDefaultVolume());
	else
		mGameData->getSoundPlayer().setVolume(std::stof(soundVolume));

	std::string musicVolume = volumeNode.getAttribute("musicvolume").toString();
	if (musicVolume == defaultVolume)
		mGameData->getMusicPlayer().setVolume(mGameData->getMusicPlayer().getDefaultVolume());
	else
		mGameData->getMusicPlayer().setVolume(std::stof(musicVolume));
}

void XmlAudioParser::parseMusicTheme(const Xml& audioNode)
{
	const Xml startThemeNode = audioNode.getChild("starttheme");
	const std::string themeFilePath = "music/" + startThemeNode.getAttribute("filename").toString();
	mGameData->getMusicPlayer().play(themeFilePath);
}

}
