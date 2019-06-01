#ifndef POPHEAD_AUDIO_SOUNDDATAHOLDER_H_
#define POPHEAD_AUDIO_SOUNDDATAHOLDER_H_

#include "soundData.hpp"
#include <string>

namespace PopHead {
namespace Audio {

class SoundDataHolder
{
public:
	SoundData getSoundData(const std::string& filePath);
};

}}

#endif