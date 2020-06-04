#include "pch.hpp"
#include "audioSystem.hpp"
#include "arcadeMode.hpp"
#include "ECS/Components/charactersComponents.hpp"
#include "ECS/Components/physicsComponents.hpp"
#include "ECS/Components/audioComponents.hpp"
#include "ECS/entityUtil.hpp"
#include "Audio/Music/musicPlayer.hpp"
#include "Audio/Sound/soundPlayer.hpp"

namespace ph::system {

AudioSystem::AudioSystem(entt::registry& registry)
	:System(registry)
{
	mSoundDistancesFromPlayer.reserve(10);
}

using namespace component;

void AudioSystem::update(float dt)
{
	PH_PROFILE_FUNCTION();

	if(sPause || !isPlayerOnScene()) return;

	Vec2 playerPos = getPlayerCenterPos();

	// play and destroy spatial sounds
	SoundPlayer::setListenerPosition(playerPos);
	mRegistry.view<SpatialSound, BodyRect>().each([&]
	(auto entity, const auto& spatialSound, auto body)
	{
		SoundPlayer::playSpatialSound(spatialSound.filepath, body.center());
		mRegistry.remove<SpatialSound>(entity);
	});

	// play and destroy ambient sounds
	mRegistry.view<AmbientSound>().each([&]
	(auto entity, auto& ambientSound)
	{
		SoundPlayer::playAmbientSound(ambientSound.filepath);
		mRegistry.remove<AmbientSound>(entity);
	});

	// for scenes without music
	if (!MusicPlayer::hasMusicState("fight") || !MusicPlayer::hasMusicState("exploration"))
		return;

	// get the closest enemy distance from player
	float theClosestEnemyDistanceFromPlayer = 1000;
	mRegistry.view<Damage, BodyRect>().each([&](auto, auto body)
	{
		Vec2 enemyPos = body.center();
		float enemyDistanceFromPlayer = Math::distanceBetweenPoints(enemyPos, playerPos);
		if(theClosestEnemyDistanceFromPlayer > enemyDistanceFromPlayer)
			theClosestEnemyDistanceFromPlayer = enemyDistanceFromPlayer;
	});

	// switch themes if they should be switched
	constexpr float distanceToEnemyToSwitchToAttackTheme = 270.f;
	constexpr float distanceToEnemyToSwitchToExplorationTheme = 350.f;

	Theme themeTypeWhichShouldBePlayed;
	if(theClosestEnemyDistanceFromPlayer < distanceToEnemyToSwitchToAttackTheme)
		themeTypeWhichShouldBePlayed = Theme::Fight;
	else if(theClosestEnemyDistanceFromPlayer > distanceToEnemyToSwitchToExplorationTheme)
		themeTypeWhichShouldBePlayed = Theme::Exploration;
	else
		themeTypeWhichShouldBePlayed = mCurrentlyPlayerTheme;

	if(themeTypeWhichShouldBePlayed != mCurrentlyPlayerTheme) 
	{
		mCurrentlyPlayerTheme = themeTypeWhichShouldBePlayed;
		if(mCurrentlyPlayerTheme == Theme::Fight)
			MusicPlayer::playFromMusicState("fight");
		else
			MusicPlayer::playFromMusicState("exploration");
	}
}

}
