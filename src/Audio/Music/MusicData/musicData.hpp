#ifndef POPHEAD_AUDIO_MUSICDATA_H_
#define POPHEAD_AUDIO_MUSICDATA_H_

#include <string>

namespace PopHead {
namespace Audio {

struct MusicData
{
	float volumeMultiplier;
	bool loop;

	MusicData(float volumeMultiplier = 1.f, bool loop = false);
};

}}



#endif // !POPHEAD_AUDIO_MUSICDATA_H_

