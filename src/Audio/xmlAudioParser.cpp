#include "pch.hpp"
#include "xmlAudioParser.hpp"
#include "Music/musicPlayer.hpp"
#include "Sound/soundPlayer.hpp"
#include "Music/musicStateMachine.hpp"

namespace ph {

void parseAudioXmlFile(const std::string& filePath)
{
	PH_LOG_INFO("Music file (" + filePath + ") is being parsed.");

	Xml audioFile;
	PH_ASSERT_CRITICAL(audioFile.loadFromFile(filePath), "scene audio file \"" + filePath + "\" wasn't loaded correctly!");
	const Xml audioNode = *audioFile.getChild("audio");

	// parse sound mute
	const auto volumeNode = audioNode.getChild("mute");
	bool soundMute = volumeNode->getAttribute("soundmute")->toBool();
	SoundPlayer::setSceneMute(soundMute);

	// parse start theme
	const auto startThemeNode = audioNode.getChild("starttheme");
	const std::string filepath = "music/" + startThemeNode->getAttribute("filename")->toString();
	MusicPlayer::playFromFile(filepath);

	// parse music states
	const auto musicStateNodes = audioNode.getChildren("musicstate");
	auto& musicStateMachine = MusicPlayer::getMusicStateMachine();

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
