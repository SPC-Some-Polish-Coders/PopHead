#include "pch.hpp"
#include "musicPlayer.hpp"
#include "Utilities/random.hpp"

namespace ph::MusicPlayer {

struct MusicData
{
	float volumeMultiplier;
	bool loop;
};

namespace {
	std::map<std::string, MusicData> themesData;
	std::string currentThemeFilePath;
	MusicData currentThemeData;

	std::map<std::string, MusicState> musicStates;

	std::unique_ptr<sf::Music> music;
	float musicVolume = 50.f;
	bool isPlayingFromMusicState = false;
}

void init()
{
	music = std::make_unique<sf::Music>();

	Xml musicDataXml;
	PH_ASSERT_CRITICAL(musicDataXml.loadFromFile("resources/music/musicData.xml"), "\"resources/music/musicData.xml\" wasn't loaded correctly!");
	auto musicDataNode = musicDataXml.getChild("musicdata");
	std::vector<Xml> themeNodes = musicDataNode->getChildren("theme");
	for(const auto& themeNode : themeNodes) 
	{
		std::string filePath = "music/" + themeNode.getAttribute("filename")->toString();
		float volumeMultiplier = themeNode.getAttribute("volumemultiplier")->toFloat();
		bool loop = themeNode.getAttribute("loop")->toBool();
		themesData[filePath] = MusicData{volumeMultiplier, loop};
	}
}

void shutdown()
{
	music.reset(Null);
}

static MusicData getMusicData(const std::string& filePath)
{
	auto found = themesData.find(filePath);
	PH_ASSERT_CRITICAL(found != themesData.end(), "MusicData with filepath \"" + filePath + "\" was not found.");
	currentThemeData = found->second;
	return found->second;
}

static void adaptVolume(float volumeMultiplier = 1.f)
{
	music->setVolume(musicVolume * getMusicData(currentThemeFilePath).volumeMultiplier * volumeMultiplier);
}

void playFromFile(const std::string& filePath)
{
	if (filePath == currentThemeFilePath)
	{
		adaptVolume();
		return;
	}

	currentThemeFilePath = filePath;

	MusicData currentThemeData = getMusicData(filePath);
	std::string fullFilePath = "resources/" + filePath;

	adaptVolume();
	music->openFromFile(fullFilePath);
	music->setLoop(currentThemeData.loop);
	music->play();
}

void playFromMusicState(const std::string& stateName)
{
	auto found = musicStates.find(stateName);
	PH_ASSERT_CRITICAL(found != musicStates.end(), "Music state \"" + stateName + "\" does not exist!");

	std::string filePath;
	float volumeMultiplier;

	auto numberOfThemes = found->second.filepaths.size();
	if(numberOfThemes == 1)
	{
		filePath = found->second.filepaths[0];
		volumeMultiplier = found->second.volumeMultipliers[0];
	}
	else 
	{
		size_t indexOfThemeToPlay = Random::generateNumber((size_t)0, numberOfThemes - 1);
		filePath = found->second.filepaths[indexOfThemeToPlay];
		volumeMultiplier = found->second.volumeMultipliers[indexOfThemeToPlay];
	}

	if(filePath == currentThemeFilePath) 
	{
		adaptVolume(volumeMultiplier);
		return;
	}
	else
	{
		currentThemeFilePath = filePath;

		MusicData currentThemeData = getMusicData(filePath);
		std::string fullFilePath = "resources/" + filePath;

		adaptVolume(volumeMultiplier);
		music->openFromFile(fullFilePath);
		music->setLoop(currentThemeData.loop);
		music->play();
	}
}

void addMusicState(const std::string& stateName, MusicState& state)
{
	musicStates[stateName] = state;
}

bool hasMusicState(const std::string& musicStateName)
{
	return musicStates.find(musicStateName) != musicStates.cend();
}

void clearMusicStates()
{
	musicStates.clear();
}

void stop()
{
	music->stop();
}

void setPaused(bool pause)
{
	pause ? music->pause() : music->play();
}

void setVolume(float volume)
{
	musicVolume = volume;
	music->setVolume(volume * currentThemeData.volumeMultiplier);
}

float getVolume()
{
	return musicVolume;
}

}

