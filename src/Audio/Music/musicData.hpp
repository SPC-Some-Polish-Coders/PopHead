#pragma once

#include <string>
#include <map>

namespace ph {

struct MusicData
{
	float mVolumeMultiplier;
	bool mLoop;

	MusicData(float volumeMultiplier = 1.f, bool loop = false);
};

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
