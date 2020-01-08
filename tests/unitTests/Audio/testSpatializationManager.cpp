#include <catch.hpp>

#include "Audio/Sound/spatializationManager.hpp"

namespace ph {

TEST_CASE("Spatialization is properly computed", "[Audio][SpatializationManager]")
{
	SpatializationManager spatializationManager;
	sf::Vector2f soundPosition(100.f, 150.f);
	sf::Vector2f listenerPosition(100.f, 100.f);
	SoundData soundData;
	float volume = 20.f;

	spatializationManager.setListenerPosition(listenerPosition);

	SECTION("Listener is out of hearable area")
	{
		soundData.mMaximalHearableDistance = 20.f;
		CHECK(spatializationManager.getSpatialVolume(soundData, soundPosition, volume) == 0.f);
	}

	SECTION("Get maximal volume")
	{
		soundData.mVolumeMultiplier = 2.f;
		soundData.mMaximalFullVolumeDistance = 60.f;
		CHECK(spatializationManager.getSpatialVolume(soundData, soundPosition, volume) == 40.f);
	}

	SECTION("Get standard range spatialization")
	{
		//soundData.mMaximalHearableDistance = 100.f;
		//CHECK(spatializationManager.getSpatialVolume(soundData, soundPosition, volume) == 2.f);
	}
}

}