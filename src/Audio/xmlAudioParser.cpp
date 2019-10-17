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
	parseStartTheme(audioNode);
	parseMusicStates(audioNode);
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

void XmlAudioParser::parseStartTheme(const Xml& audioNode)
{
	//TODO: What should we do with volumeMultiplier parameter

	const Xml startThemeNode = audioNode.getChild("starttheme");
	const std::string filepath = "music/" + startThemeNode.getAttribute("filename").toString();
	mGameData->getMusicPlayer().playFromFile(filepath);
}

void XmlAudioParser::parseMusicStates(const Xml& audioNode)
{
	const auto musicStateNodes = audioNode.getChildren("musicstate");
	auto& musicStateMachine = mGameData->getMusicPlayer().getMusicStateMachine();

	for(const auto& musicStateNode : musicStateNodes)
	{
		MusicState musicState;
		const auto themeNodes = musicStateNode.getChildren("theme");

		for(const auto& theme : themeNodes)
		{
			const std::string themeFilePath = "music/" + theme.getAttribute("filename").toString();
			musicState.filepaths.emplace_back(themeFilePath);

			const float volumeMultiplier = theme.hasAttribute("volumeMultiplier") ? theme.getAttribute("volumeMultiplier").toFloat() : 1.f;
			musicState.volumeMultipliers.emplace_back(volumeMultiplier);
		}

		const std::string musicStateName = musicStateNode.getAttribute("name").toString();
		musicStateMachine.addState(musicStateName, musicState);
	}
}

}
