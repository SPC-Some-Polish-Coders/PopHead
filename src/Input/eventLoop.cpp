#include "eventLoop.hpp"

#include "gameData.hpp"

namespace ph {

void EventLoop::init(GameData* gameData)
{
	gameData->getRenderWindow().setKeyRepeatEnabled(false);
}

void EventLoop::eventLoop(GameData* gameData)
{
	clear();
	sf::Event event;

	while(gameData->getRenderWindow().pollEvent(event))
	{
		switch(event.type)
		{
		case sf::Event::KeyPressed:
			mIsKeyJustPressed = true;
			mPressedKey = event.key.code;
			break;

		case sf::Event::KeyReleased:
			mIsKeyJustReleased = true;
			mReleasedKey = event.key.code;
			break;

		case sf::Event::MouseButtonPressed:
			mIsMouseButtonJustPressed = true;
			mMouseButton = event.mouseButton.button;
			break;

		case sf::Event::MouseButtonReleased:
			mIsMouseButtonJustReleased = true;
			mMouseButton = event.mouseButton.button;
			break;

		case sf::Event::MouseMoved:
			mHasMouseJustMoved = true;
			break;
		}
	}
}

void EventLoop::clear()
{
	mPressedKey = sf::Keyboard::Unknown;
	mReleasedKey = sf::Keyboard::Unknown;
	mMouseButton = sf::Mouse::ButtonCount;
	mIsKeyJustPressed = false;
	mIsKeyJustReleased = false;
	mIsMouseButtonJustPressed = false;
	mIsMouseButtonJustReleased = false;
	mHasMouseJustMoved = false;
}

}