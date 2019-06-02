#ifndef POPHEAD_AUDIO_SOUND_SPATIALIZATION_SPATIALSOUNDVOLUMECALCULATOR_H_
#define POPHEAD_AUDIO_SOUND_SPATIALIZATION_SPATIALSOUNDVOLUMECALCULATOR_H_

#include <SFML/Graphics.hpp>
#include "Audio/Sound/SoundData/SoundData.hpp"

namespace PopHead {
namespace Audio {

class SpatializationManager
{
public:
	void setListenerPosition(sf::Vector2f listenerPosition) { mListenerPosition = listenerPosition; }
	float getSpatialVolume(SoundData, const sf::Vector2f& soundPosition, float volume);
private:
	void init(SoundData, const sf::Vector2f& soundPosition, float volume);
	float getDistanceBetweenListenerAndSoundSource();
	bool isListenerOutOfHearableArea();
	bool isListenerVeryCloseToSoundSource();
	float getMaximalVolume();
	float getVolumeForListenerInDistanceScope();
	float getSpatializationFactor();

private:
	sf::Vector2f mListenerPosition;

	SoundData mSoundData;
	sf::Vector2f mSoundPosition;
	float mVolume;
	float mDistanceBetweenListenerAndSoundSource;
};

}}

#endif