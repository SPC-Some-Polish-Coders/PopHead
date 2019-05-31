#include "musicResource.hpp"

using PopHead::Audio::MusicResource;

MusicResource::MusicResource(float volumeMultiplier, bool loop)
	: volumeMultiplier(volumeMultiplier)
	, loop(loop)
{
}
