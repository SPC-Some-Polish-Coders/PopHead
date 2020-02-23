#include "soundBufferHolder.hpp"
#include "Logs/logs.hpp"
#include <memory>
#include <unordered_map>

namespace ph {

static std::unordered_map<std::string, std::unique_ptr<sf::SoundBuffer>> soundBuffers;

bool loadSoundBuffer(const std::string& filePath)
{
	std::string fullFilePath = "resources/" + filePath;
	if(soundBuffers.find(fullFilePath) != soundBuffers.end())
		return true;
	auto soundBuffer = std::make_unique<sf::SoundBuffer>();
	if(soundBuffer->loadFromFile(fullFilePath))
	{
		soundBuffers.insert(std::make_pair(fullFilePath, std::move(soundBuffer)));
		return true;
	}
	else
	{
		PH_LOG_ERROR("unable to load sound buffer \"" + fullFilePath + "\"");
		return false;
	}
}

sf::SoundBuffer& getSoundBuffer(const std::string& filePath)
{
	std::string fullFilePath = "resources/" + filePath;
	auto found = soundBuffers.find(fullFilePath);
	PH_ASSERT_CRITICAL(found == soundBuffers.end(), "You try to get a sound buffer that wasn't loaded: " + fullFilePath);
	return *found->second;
}

}

