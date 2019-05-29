#ifndef POPHEAD_AUDIO_MUSICRESOURCE_H_
#define POPHEAD_AUDIO_MUSICRESOURCE_H_

#include <string>

struct MusicResource
{
	std::string filepath;
	float volumeMultiplier;
	bool loop;

	MusicResource::MusicResource(std::string filepath, float volumeMultiplier = 1.f, bool loop = false)
		:filepath(filepath)
		, volumeMultiplier(volumeMultiplier)
		, loop(loop)
	{
	}
};


#endif // !POPHEAD_AUDIO_MUSICRESOURCE_H_
