#include "musicDataHolder.hpp"

#include "Utilities/debug.hpp"

ph::MusicDataHolder::MusicDataHolder()
{
	using namespace std::string_literals;
	mAllThemesData["music/explorationTheme.ogg"s] = MusicData(1.2f, true);
}

auto ph::MusicDataHolder::getMusicData(const std::string& filePath) -> const MusicData&
{
	auto found = mAllThemesData.find(filePath);

#ifndef PH_DISTRIBUTION
	if(found == mAllThemesData.end())
		PH_EXCEPTION("MusicData with filepath \"" + filePath + "\" was not found.");
#endif // !PH_DISTRIBUTION

	mCurrentThemeData = found->second;
	return found->second;
}
