#include "musicData.hpp"

using PopHead::Audio::MusicData;

MusicData::MusicData(float volumeMultiplier, bool loop)
	: volumeMultiplier(volumeMultiplier)
	, loop(loop)
{
}
