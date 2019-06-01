#ifndef POPHEAD_AUDIO_MUSICDATA_H_
#define POPHEAD_AUDIO_MUSICDATA_H_

#include <string>

namespace PopHead {
namespace Audio {

// This is very temporary. Later we'll be loading music data from .ini file.

struct MusicData
{
	float volumeMultiplier;
	bool loop;

	MusicData(float volumeMultiplier = 1.f, bool loop = false);
};

MusicData getMusicData(const std::string& filePath);

}}



#endif // !POPHEAD_AUDIO_MUSICDATA_H_

