#include "spatializationManager.hpp"

using PopHead::Audio::SpatializationManager;
using PopHead::Audio::SoundData;

float SpatializationManager::getSpatialVolume(SoundData soundData, const sf::Vector2f& soundPosition, float volume)
{
	init(soundData, soundPosition, volume);

	float distanceBetweenListenerAndSoundSource = hypotf(abs(mListenerPosition.x - soundPosition.x), abs(mListenerPosition.y - soundPosition.y));
	float spatialVolume;

	if(distanceBetweenListenerAndSoundSource > soundData.mMax)
		return 0.f;
	else if(distanceBetweenListenerAndSoundSource < soundData.mMin)
		return getMaximalVolume();
	else {
		float scope = soundData.mMax - soundData.mMin;
		spatialVolume = volume * soundData.mVolumeMultiplier * ((scope - (distanceBetweenListenerAndSoundSource - soundData.mMin)) / scope);
	}

	return spatialVolume;
}

void SpatializationManager::init(SoundData soundData, const sf::Vector2f& soundPosition, float volume)
{
	mSoundData = soundData;
	mSoundPosition = soundPosition;
	mVolume = volume;
}

float SpatializationManager::getMaximalVolume()
{
	return mVolume * mSoundData.mVolumeMultiplier;
}
