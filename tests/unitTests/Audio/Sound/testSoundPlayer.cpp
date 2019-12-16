#include <catch.hpp>

#include "Audio/Sound/soundPlayer.hpp"

namespace ph {

TEST_CASE("Ambient sound can be played", "[Audio][SoundPlayer]")
{
	SoundPlayer soundPlayer;

	CHECK_NOTHROW(soundPlayer.playAmbientSound("sounds/pistolShot.ogg"));
}

TEST_CASE("Spatial sound can be played", "[Audio][SoundPlayer]")
{
	SoundPlayer soundPlayer;
	sf::Vector2f soundPosition(100.f, 100.f);
	sf::Vector2f playerPosition(150.f, 150.f);
	soundPlayer.setListenerPosition(playerPosition);

	CHECK_NOTHROW(soundPlayer.playSpatialSound("sounds/zombieGrowl1.ogg", soundPosition));
}

TEST_CASE("Mute is set and can be read", "[Audio][SoundPlayer]")
{
	SoundPlayer soundPlayer;

	soundPlayer.setMuted(false);
	CHECK(soundPlayer.isMuted() == false);

	soundPlayer.setMuted(true);
	CHECK(soundPlayer.isMuted() == true);
}

TEST_CASE("Volume is set and can be read", "[Audio][SoundPlayer]")
{
	SoundPlayer soundPlayer;

	SECTION("Standard value")
	{
		soundPlayer.setVolume(65.f);
		CHECK(soundPlayer.getVolume() == 65.f);
	}
	SECTION("Value below 0")
	{
		soundPlayer.setVolume(-100.f);
		CHECK(soundPlayer.getVolume() == -100.f);
	}

	SECTION("Value above 100")
	{
		soundPlayer.setVolume(250.f);
		CHECK(soundPlayer.getVolume() == 250.f);
	}
}

}
