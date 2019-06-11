#ifndef POPHEAD_AUDIO_SOUNDDATAHOLDER_H_
#define POPHEAD_AUDIO_SOUNDDATAHOLDER_H_

#include "soundData.hpp"
#include <string>
#include <map>

namespace ph {
namespace Audio {

class SoundDataHolder
{
public:
	SoundDataHolder();

	SoundData getSoundData(const std::string& filePath);

private:
	std::map<std::string, SoundData> mAllSoundsData;
};

}}

#endif