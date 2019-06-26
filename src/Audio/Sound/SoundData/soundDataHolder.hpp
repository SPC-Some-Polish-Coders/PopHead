#pragma once

#include "soundData.hpp"
#include <string>
#include <map>

namespace ph {

class SoundDataHolder
{
public:
	SoundDataHolder();

	auto getSoundData(const std::string& filePath) -> SoundData;

private:
	std::map<std::string, SoundData> mAllSoundsData;
};

}
