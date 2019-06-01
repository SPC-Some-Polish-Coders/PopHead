#include "musicData.hpp"

using PopHead::Audio::MusicData;

MusicData PopHead::Audio::getMusicData(const std::string& filePath)
{
	if(filePath == "resources/music/explorationTheme.ogg")
		return MusicData(0.2f, true);
	else
		return MusicData();
}

MusicData::MusicData(float volumeMultiplier, bool loop)
	: volumeMultiplier(volumeMultiplier)
	, loop(loop)
{
}