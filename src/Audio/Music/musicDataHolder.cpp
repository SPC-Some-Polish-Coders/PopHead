#include "musicDataHolder.hpp"

using PopHead::Audio::MusicDataHolder;
using PopHead::Audio::MusicData;

MusicDataHolder::MusicDataHolder()
{
}

MusicData MusicDataHolder::getMusicData(const std::string& filePath)
{
	if(filePath == "resources/music/explorationTheme.ogg")
		return MusicData(0.2f, true);
	else
		return MusicData();
}