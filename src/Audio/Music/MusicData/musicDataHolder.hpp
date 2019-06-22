#pragma once

#include "musicData.hpp"
#include <string>
#include <map>

namespace ph {

class MusicDataHolder
{
public:
	MusicDataHolder();

	auto getMusicData (const std::string& filePath) -> const MusicData&;
	auto getCurrentThemeData() -> const MusicData& { return mCurrentThemeData; }

private:
	std::map<std::string, MusicData> mAllThemesData;
	MusicData mCurrentThemeData;
};

}
