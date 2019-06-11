#ifndef POPHEAD_AUDIO_SOUNDDATAHOLDER_H_
#define POPHEAD_AUDIO_SOUNDDATAHOLDER_H_

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

#endif