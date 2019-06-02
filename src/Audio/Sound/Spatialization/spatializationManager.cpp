#include "spatializationManager.hpp"

using PopHead::Audio::SpatializationManager;
using PopHead::Audio::SoundData;

float SpatializationManager::getSpatialVolume(SoundData soundData, const sf::Vector2f& soundPosition, float volume)
{
	float distanceBetweenListenerAndSoundSource = hypotf(abs(mListenerPosition.x - soundPosition.x), abs(mListenerPosition.y - soundPosition.y));
	int spatialVolume;

	if(distanceBetweenListenerAndSoundSource > soundData.mMax)
		return 0.f;
	else if(distanceBetweenListenerAndSoundSource < soundData.mMin)
		spatialVolume = static_cast<int>(volume * soundData.mVolumeMultiplier);
	else {
		float scope = soundData.mMax - soundData.mMin;
		spatialVolume = static_cast<int>(volume * soundData.mVolumeMultiplier * ((scope - (distanceBetweenListenerAndSoundSource - soundData.mMin)) / scope));
	}

	return static_cast<float>(spatialVolume);
}