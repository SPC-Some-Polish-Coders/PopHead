#include "musicDataHolder.hpp"

using PopHead::Audio::MusicDataHolder;
using PopHead::Audio::MusicData;

MusicDataHolder::MusicDataHolder()
{
	mAllThemesData["music/explorationTheme.ogg"] = MusicData(1.2f, true);
}

auto MusicDataHolder::getMusicData(const std::string& filePath) -> const MusicData&
{
	auto found = mAllThemesData.find(filePath);
	mCurrentThemeData = found->second;
	return found->second;
}