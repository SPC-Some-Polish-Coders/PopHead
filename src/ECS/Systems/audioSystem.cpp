#include "audioSystem.hpp"
#include "ECS/Components/charactersComponents.hpp"
#include "ECS/Components/physicsComponents.hpp"
#include "ECS/Components/audioComponents.hpp"
#include "Audio/Music/musicPlayer.hpp"
#include "Audio/Sound/soundPlayer.hpp"
#include "Utilities/math.hpp"
#include "Utilities/profiling.hpp"
#include <SFML/System/Vector2.hpp>

namespace ph::system {

	AudioSystem::AudioSystem(entt::registry& registry, MusicPlayer& musicPlayer, SoundPlayer& soundPlayer)
		:System(registry)
		,mMusicPlayer(musicPlayer)
		,mSoundPlayer(soundPlayer)
	{
		mSoundDistancesFromPlayer.reserve(10);
	}

	void AudioSystem::update(float dt)
	{
		PH_PROFILE_FUNCTION();

		// define constants
		constexpr float distanceToEnemyToSwitchToAttackTheme = 270.f;
		constexpr float distanceToEnemyToSwitchToExplorationTheme = 350.f;

		// get player position
		sf::Vector2f playerPos;
		auto playerView = mRegistry.view<component::Player, component::BodyRect>();
		playerView.each([&playerPos](const component::Player, const component::BodyRect& body) {
			playerPos = body.rect.getCenter();
		});

		// get the closest enemy distance from player
		float theClosestEnemyDistanceFromPlayer = 1000;
		auto enemiesView = mRegistry.view<component::Damage, component::BodyRect>();
		enemiesView.each([&theClosestEnemyDistanceFromPlayer, playerPos](const component::Damage, const component::BodyRect& body) 
		{
			const sf::Vector2f enemyPos = body.rect.getCenter();
			const float enemyDistanceFromPlayer = Math::distanceBetweenPoints(enemyPos, playerPos);
			if(theClosestEnemyDistanceFromPlayer > enemyDistanceFromPlayer)
				theClosestEnemyDistanceFromPlayer = enemyDistanceFromPlayer;
		});

		// switch themes if they should be switched
		Theme themeTypeWhichShouldBePlayed;
		if(theClosestEnemyDistanceFromPlayer < distanceToEnemyToSwitchToAttackTheme)
			themeTypeWhichShouldBePlayed = Theme::Fight;
		else if(theClosestEnemyDistanceFromPlayer > distanceToEnemyToSwitchToExplorationTheme)
			themeTypeWhichShouldBePlayed = Theme::Exploration;
		else
			themeTypeWhichShouldBePlayed = mCurrentlyPlayerTheme;

		if(themeTypeWhichShouldBePlayed != mCurrentlyPlayerTheme) {
			mCurrentlyPlayerTheme = themeTypeWhichShouldBePlayed;
			if(mCurrentlyPlayerTheme == Theme::Fight)
				mMusicPlayer.playFromMusicState("fight");
			else
				mMusicPlayer.playFromMusicState("exploration");
		}

		// play and destroy ambient sounds
		auto ambientSoundsView = mRegistry.view<component::AmbientSound>();
		for(auto& entity : ambientSoundsView)
		{
			const auto& ambientSound = ambientSoundsView.get<component::AmbientSound>(entity);
			mSoundPlayer.playAmbientSound(ambientSound.filepath);
			mRegistry.remove<component::AmbientSound>(entity);
		}

		// play and destroy spatial sounds
		mSoundPlayer.setListenerPosition(playerPos);
		auto spatialSoundsView = mRegistry.view<component::SpatialSound, component::BodyRect>();
		for(auto& entity : spatialSoundsView)
		{
			const auto& [spatialSound, body] = spatialSoundsView.get<component::SpatialSound, component::BodyRect>(entity);
			mSoundPlayer.playSpatialSound(spatialSound.filepath, body.rect.getCenter());
			mRegistry.remove<component::SpatialSound>(entity);
		}
	}
}
