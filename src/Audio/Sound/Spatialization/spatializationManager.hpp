#ifndef POPHEAD_AUDIO_SOUND_SPATIALIZATION_SPATIALSOUNDVOLUMECALCULATOR_H_
#define POPHEAD_AUDIO_SOUND_SPATIALIZATION_SPATIALSOUNDVOLUMECALCULATOR_H_

#include <SFML/Graphics.hpp>
#include "Audio/Sound/SoundData/SoundData.hpp"

namespace PopHead {
namespace Audio {

class SpatializationManager
{
public:
	int getSpatialVolume(SoundData, const sf::Vector2f& soundPosition, float volume);
	void setListenerPosition(sf::Vector2f listenerPosition) { mListenerPosition = listenerPosition; }

private:
	sf::Vector2f mListenerPosition;
};

}}

#endif