#include "eventLoop.hpp"

#include "gameData.hpp"

namespace ph {

bool EventLoop::mIsKeyJustPressed;
bool EventLoop::mIsKeyJustReleased;
bool EventLoop::mIsMouseButtonJustPressed;
bool EventLoop::mIsMouseButtonJustReleased;
bool EventLoop::mHasMouseJustMoved;
sf::Keyboard::Key EventLoop::mKey;
sf::Mouse::Button EventLoop::mMouseButton;

void EventLoop::init(GameData* gameData)
{
	gameData->getRenderer().getWindow().setKeyRepeatEnabled(false);
}

void EventLoop::eventLoop(GameData* gameData)
{
	clear();
	sf::Event event;

	while(gameData->getRenderer().getWindow().pollEvent(event))
	{
		switch(event.type)
		{
		case sf::Event::KeyPressed:
			mIsKeyJustPressed = true;
			mKey = event.key.code;
			break;

		case sf::Event::KeyReleased:
			mIsKeyJustReleased = true;
			mKey = event.key.code;
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
	mIsKeyJustPressed = false;
	mIsKeyJustReleased = false;
	mIsMouseButtonJustPressed = false;
	mIsMouseButtonJustReleased = false;
	mHasMouseJustMoved = false;
	mKey = sf::Keyboard::Unknown;
	mMouseButton = sf::Mouse::ButtonCount;
}

}