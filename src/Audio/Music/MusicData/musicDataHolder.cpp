#include "musicDataHolder.hpp"

using PopHead::Audio::MusicDataHolder;
using PopHead::Audio::MusicData;

MusicDataHolder::MusicDataHolder()
{
	mAllThemesData["resources/music/explorationTheme.ogg"] = MusicData(0.2f, true);
}

MusicData MusicDataHolder::getMusicData(const std::string& filePath)
{
	auto found = mAllThemesData.find(filePath);
	mCurrentThemeData = found->second;
	return found->second;
}