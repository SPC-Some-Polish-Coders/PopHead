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
	parseSoundMute(audioNode);
	parseMusicTheme(audioNode);
}

void XmlAudioParser::parseSoundMute(const Xml& audioNode)
{
	const Xml volumeNode = audioNode.getChild("mute");
	bool soundMute = volumeNode.getAttribute("soundmute").toBool();

	if(soundMute)
		mGameData->getSoundPlayer().setSceneMute(true);
	else
		mGameData->getSoundPlayer().setSceneMute(false);
}

void XmlAudioParser::parseMusicTheme(const Xml& audioNode)
{
	const auto themeNodes = audioNode.getChildren("theme");
	std::string startTheme;

	for (const auto& theme : themeNodes)
	{
		const std::string themeFilePath = "music/" + theme.getAttribute("filename").toString();
		if (startTheme.empty())
			startTheme = themeFilePath;
		if (theme.hasAttribute("volumeMultiplier"))
			mGameData->getMusicPlayer().setVolumeMultiplierForTheme(themeFilePath, theme.getAttribute("volumeMultiplier").toFloat());
	}
	mGameData->getMusicPlayer().play(startTheme);
}

}
