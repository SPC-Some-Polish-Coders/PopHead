#include "audioSystem.hpp"
#include "ECS/Components/charactersComponents.hpp"
#include "ECS/Components/physicsComponents.hpp"
#include "Audio/Music/musicPlayer.hpp"
#include "Audio/Sound/soundPlayer.hpp"
#include "Utilities/math.hpp"
#include <SFML/System/Vector2.hpp>

namespace ph::system {

	AudioSystem::AudioSystem(entt::registry& registry, MusicPlayer& musicPlayer, SoundPlayer& soundPlayer)
		:System(registry)
		,mMusicPlayer(musicPlayer)
		,mSoundPlayer(soundPlayer)
	{
	}

	void AudioSystem::update(float dt)
	{
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

		// TODO: Some delay on changing theme

		// change theme
		Theme themeTypeWhichShouldBePlayed = theClosestEnemyDistanceFromPlayer < 230 ? Theme::Fight : Theme::Exploration;
		if(themeTypeWhichShouldBePlayed != mCurrentlyPlayerTheme) {
			mCurrentlyPlayerTheme = themeTypeWhichShouldBePlayed;
			if(mCurrentlyPlayerTheme == Theme::Fight)
				mMusicPlayer.playFromMusicState("fight");
			else
				mMusicPlayer.playFromMusicState("exploration");
		}
	}
}