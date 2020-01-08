#include "xmlAudioParser.hpp"
#include "Audio/Music/musicPlayer.hpp"
#include "Audio/Sound/soundPlayer.hpp"
#include "Utilities/xml.hpp"
#include "Logs/logs.hpp"

namespace ph {

void XmlAudioParser::parseFile(SoundPlayer& soundPlayer, MusicPlayer& musicPlayer, const std::string& filePath)
{
	PH_LOG_INFO("Music file (" + filePath + ") is being parsed.");

	mSoundPlayer = &soundPlayer;
	mMusicPlayer = &musicPlayer;

	Xml audioFile;
	PH_ASSERT_CRITICAL(audioFile.loadFromFile(filePath), "scene audio file \"" + filePath + "\" wasn't loaded correctly!");
	const Xml audioNode = *audioFile.getChild("audio");

	parseSoundMute(audioNode);
	parseStartTheme(audioNode);
	parseMusicStates(audioNode);
}

void XmlAudioParser::parseSoundMute(const Xml& audioNode)
{
	const auto volumeNode = audioNode.getChild("mute");
	bool soundMute = volumeNode->getAttribute("soundmute")->toBool();
	mSoundPlayer->setSceneMute(soundMute);
}

void XmlAudioParser::parseStartTheme(const Xml& audioNode)
{
	//TODO: What should we do with volumeMultiplier parameter

	const auto startThemeNode = audioNode.getChild("starttheme");
	const std::string filepath = "music/" + startThemeNode->getAttribute("filename")->toString();
	mMusicPlayer->playFromFile(filepath);
}

void XmlAudioParser::parseMusicStates(const Xml& audioNode)
{
	const auto musicStateNodes = audioNode.getChildren("musicstate");
	auto& musicStateMachine = mMusicPlayer->getMusicStateMachine();

	for(const auto& musicStateNode : musicStateNodes)
	{
		MusicState musicState;
		const auto themeNodes = musicStateNode.getChildren("theme");

		for(const auto& theme : themeNodes)
		{
			const std::string themeFilePath = "music/" + theme.getAttribute("filename")->toString();
			musicState.filepaths.emplace_back(themeFilePath);

			auto volumeMultiplier = theme.getAttribute("volumeMultiplier");
			musicState.volumeMultipliers.emplace_back(volumeMultiplier ? volumeMultiplier->toFloat() : 1.f);
		}

		const std::string musicStateName = musicStateNode.getAttribute("name")->toString();
		musicStateMachine.addState(musicStateName, musicState);
	}
}

}
