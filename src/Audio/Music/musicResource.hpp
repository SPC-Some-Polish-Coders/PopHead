#ifndef POPHEAD_AUDIO_MUSICRESOURCE_H_
#define POPHEAD_AUDIO_MUSICRESOURCE_H_

#include <string>

namespace PopHead {
namespace Audio {


struct MusicResource
{
	float volumeMultiplier;
	bool loop;

	MusicResource(float volumeMultiplier = 1.f, bool loop = false);
};


}}

#endif // !POPHEAD_AUDIO_MUSICRESOURCE_H_
