#include "spatializationManager.hpp"

using PopHead::Audio::SpatializationManager;
using PopHead::Audio::SoundData;

int SpatializationManager::getSpatialVolume(SoundData soundData, const sf::Vector2f& soundPosition, float volume)
{
	float distance = hypotf(abs(mListenerPosition.x - soundPosition.x), abs(mListenerPosition.y - soundPosition.y));

	if(distance > soundData.mMax)
		return 0;
	else if(distance < soundData.mMin)
		return static_cast<int>(volume * soundData.mVolumeMultiplier);
	else {
		float scope = soundData.mMax - soundData.mMin;
		return static_cast<int>(volume * soundData.mVolumeMultiplier * ((scope - (distance - soundData.mMin)) / scope));
	}
}