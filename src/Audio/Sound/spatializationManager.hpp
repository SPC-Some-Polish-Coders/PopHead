#pragma once

#include <SFML/Graphics.hpp>
#include "Audio/Sound/soundData.hpp"

namespace ph {

class SpatializationManager
{
public:
	void setListenerPosition(const Vec2 listenerPosition);
	float getSpatialVolume(const SoundData, const Vec2 soundPosition, const float volume);
private:
	void init(const SoundData, const Vec2 soundPosition, const float volume);
	float getDistanceBetweenListenerAndSoundSource();
	bool isListenerOutOfHearableArea();
	bool isListenerVeryCloseToSoundSource();
	float getMaximalVolume();
	float getVolumeForListenerInDistanceScope();
	float getSpatializationFactor();

private:
	SoundData mSoundData;
	Vec2 mListenerPosition;
	Vec2 mSoundPosition;
	float mVolume;
	float mDistanceBetweenListenerAndSoundSource;
};

}
