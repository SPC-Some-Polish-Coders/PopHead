#include "States/GameState/gameState.hpp"

#include "SFML/Graphics.hpp"
#include "World/Entity/Objects/Characters/player.hpp"
#include "World/Entity/Objects/Map/map.hpp"
#include "World/Entity/Objects/shapeWithCollision.hpp"
#include "World/Entity/Objects/Characters/enemies/zombie.hpp"
#include "World/Entity/Objects/Characters/npc.hpp"
#include "gameData.hpp"
#include "Utilities/math.hpp"

namespace ph {

GameState::GameState(GameData* const gameData)
	:State(gameData)
{
	playMusic();
}

void GameState::playMusic()
{
	mGameData->getMusicPlayer().play("music/explorationTheme.ogg");
}

void GameState::input()
{
	mRoot.input();
	windowMinimalizeAndMaximalizeShortcut();
}

void GameState::windowMinimalizeAndMaximalizeShortcut()
{
	enum class WindowSizeState {fullScreen, notFullScreen};
	static WindowSizeState windowSizeState = WindowSizeState::fullScreen;

	if(mGameData->getInput().getKeyboard().isKeyJustPressed(sf::Keyboard::F11)) {
		switch(windowSizeState)
		{
		case WindowSizeState::fullScreen:
			windowSizeState = WindowSizeState::notFullScreen;
			mGameData->getRenderer().getWindow().create(sf::VideoMode(1000, 750), "PopHead", sf::Style::Default);
			break;
		case WindowSizeState::notFullScreen:
			windowSizeState = WindowSizeState::fullScreen;
			mGameData->getRenderer().getWindow().create(sf::VideoMode(), "PopHead", sf::Style::Fullscreen);
			break;
		}
	}
}

void GameState::update(sf::Time delta)
{
	mRoot.update(delta);
}

}