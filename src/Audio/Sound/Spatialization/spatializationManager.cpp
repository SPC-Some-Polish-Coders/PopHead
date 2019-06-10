#include "spatializationManager.hpp"

using PopHead::Audio::SpatializationManager;
using PopHead::Audio::SoundData;

float SpatializationManager::getSpatialVolume(const SoundData soundData, const sf::Vector2f soundPosition, const float volume)
{
	init(soundData, soundPosition, volume);

	if(isListenerOutOfHearableArea())
		return 0.f;
	else if(isListenerVeryCloseToSoundSource())
		return getMaximalVolume();
	else
		return getVolumeForListenerInDistanceScope();
}

void SpatializationManager::init(const SoundData soundData, const sf::Vector2f soundPosition, const float volume)
{
	mSoundData = soundData;
	mSoundPosition = soundPosition;
	mVolume = volume;
	mDistanceBetweenListenerAndSoundSource = getDistanceBetweenListenerAndSoundSource();
}

float SpatializationManager::getDistanceBetweenListenerAndSoundSource()
{
	float legX = abs(mListenerPosition.x - mSoundPosition.x);
	float legY = abs(mListenerPosition.y - mSoundPosition.y);
	return hypotf(legX, legY);
}

bool SpatializationManager::isListenerOutOfHearableArea()
{
	return mDistanceBetweenListenerAndSoundSource > mSoundData.mMaximalHearableDistance;
}

bool SpatializationManager::isListenerVeryCloseToSoundSource()
{
	return mDistanceBetweenListenerAndSoundSource < mSoundData.mMaximalFullVolumeDistance;
}

float SpatializationManager::getMaximalVolume()
{
	return mVolume * mSoundData.mVolumeMultiplier;
}

float SpatializationManager::getVolumeForListenerInDistanceScope()
{
	return getMaximalVolume() * getSpatializationFactor();
}

float SpatializationManager::getSpatializationFactor()
{
	float scope = mSoundData.mMaximalHearableDistance - mSoundData.mMaximalFullVolumeDistance;
	float distanceBetweenListenerAndMaximalVolumeArea = mDistanceBetweenListenerAndSoundSource - mSoundData.mMaximalFullVolumeDistance;
	float distanceBetweenListenerAndHearableAreaBound = scope - distanceBetweenListenerAndMaximalVolumeArea;
	float spatializationFactor = distanceBetweenListenerAndHearableAreaBound / scope;
	return spatializationFactor;
}
