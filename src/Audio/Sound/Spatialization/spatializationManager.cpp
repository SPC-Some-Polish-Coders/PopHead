#include "SpatializationManager.hpp"

float ph::SpatializationManager::getSpatialVolume(const SoundData soundData, const sf::Vector2f soundPosition, const float volume)
{
	init(soundData, soundPosition, volume);

	if(isListenerOutOfHearableArea())
		return 0.f;
	else if(isListenerVeryCloseToSoundSource())
		return getMaximalVolume();
	else
		return getVolumeForListenerInDistanceScope();
}

void ph::SpatializationManager::init(const SoundData soundData, const sf::Vector2f soundPosition, const float volume)
{
	mSoundData = soundData;
	mSoundPosition = soundPosition;
	mVolume = volume;
	mDistanceBetweenListenerAndSoundSource = getDistanceBetweenListenerAndSoundSource();
}

float ph::SpatializationManager::getDistanceBetweenListenerAndSoundSource()
{
	float legX = abs(mListenerPosition.x - mSoundPosition.x);
	float legY = abs(mListenerPosition.y - mSoundPosition.y);
	return hypotf(legX, legY);
}

bool ph::SpatializationManager::isListenerOutOfHearableArea()
{
	return mDistanceBetweenListenerAndSoundSource > mSoundData.mMaximalHearableDistance;
}

bool ph::SpatializationManager::isListenerVeryCloseToSoundSource()
{
	return mDistanceBetweenListenerAndSoundSource < mSoundData.mMaximalFullVolumeDistance;
}

float ph::SpatializationManager::getMaximalVolume()
{
	return mVolume * mSoundData.mVolumeMultiplier;
}

float ph::SpatializationManager::getVolumeForListenerInDistanceScope()
{
	return getMaximalVolume() * getSpatializationFactor();
}

float ph::SpatializationManager::getSpatializationFactor()
{
	float scope = mSoundData.mMaximalHearableDistance - mSoundData.mMaximalFullVolumeDistance;
	float distanceBetweenListenerAndMaximalVolumeArea = mDistanceBetweenListenerAndSoundSource - mSoundData.mMaximalFullVolumeDistance;
	float distanceBetweenListenerAndHearableAreaBound = scope - distanceBetweenListenerAndMaximalVolumeArea;
	float spatializationFactor = distanceBetweenListenerAndHearableAreaBound / scope;
	return spatializationFactor;
}
