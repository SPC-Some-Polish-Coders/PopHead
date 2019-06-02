#include "spatializationManager.hpp"

using PopHead::Audio::SpatializationManager;
using PopHead::Audio::SoundData;

float SpatializationManager::getSpatialVolume(SoundData soundData, const sf::Vector2f& soundPosition, float volume)
{
	init(soundData, soundPosition, volume);

	if(isListenerOutOfHearableArea())
		return 0.f;
	else if(isListenerVeryCloseToSoundSource())
		return getMaximalVolume();
	else
		return getVolumeForListenerInDistanceScope();
}

void SpatializationManager::init(SoundData soundData, const sf::Vector2f& soundPosition, float volume)
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
	return mDistanceBetweenListenerAndSoundSource > mSoundData.mMax;
}

bool SpatializationManager::isListenerVeryCloseToSoundSource()
{
	return mDistanceBetweenListenerAndSoundSource < mSoundData.mMin;
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
	float scope = mSoundData.mMax - mSoundData.mMin;
	float distanceBetweenListenerAndMaximalVolumeArea = mDistanceBetweenListenerAndSoundSource - mSoundData.mMin;
	float distanceBetweenListenerAndHearableAreaBound = scope - distanceBetweenListenerAndMaximalVolumeArea;
	float spatializationFactor = distanceBetweenListenerAndHearableAreaBound / scope;
	return spatializationFactor;
}
