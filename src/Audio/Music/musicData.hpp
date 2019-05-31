#ifndef POPHEAD_AUDIO_MUSICDATA_H_
#define POPHEAD_AUDIO_MUSICDATA_H_

#include "musicResource.hpp"

namespace PopHead {
namespace Audio {

// This is very temporary. Later we'll be loading music data from .ini file.

MusicResource getMusicData(const std::string& filePath)
{
	if(filePath == "resources/music/explorationTheme.ogg")
		return MusicResource(0.2f, true);
	else
		return MusicResource();
}


}}



#endif // !POPHEAD_AUDIO_MUSICDATA_H_

