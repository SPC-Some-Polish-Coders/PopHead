#include "States/state.hpp"

#include "World/Entity/entityType.hpp"
#include "gameData.hpp"
#include <SFML/Graphics.hpp>

namespace ph {

State::State(GameData* const gameData)
	:mRoot(EntityType::none, gameData, "root")
	,mGameData(gameData)
	,mSceneParser(gameData, mRoot, "scene.xml")
	,mHide(false)
	,mPause(false)
{
}

void State::input()
{
	mRoot.input();
	windowMinimalizeAndMaximalizeShortcut();
}

void State::update(sf::Time delta)
{
	mRoot.update(delta);
}

void State::windowMinimalizeAndMaximalizeShortcut() const
{
	enum class WindowSizeState { fullScreen, notFullScreen };
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

}