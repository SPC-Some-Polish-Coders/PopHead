#include "eventLoop.hpp"

#include "Base/gameData.hpp"

bool ph::EventLoop::mIsKeyJustPressed;
bool ph::EventLoop::mIsKeyJustReleased;
bool ph::EventLoop::mIsMouseButtonJustPressed;
bool ph::EventLoop::mIsMouseButtonJustReleased;
bool ph::EventLoop::mHasMouseJustMoved;
sf::Keyboard::Key ph::EventLoop::mKey;
sf::Mouse::Button ph::EventLoop::mMouseButton;

void ph::EventLoop::init(GameData* gameData)
{
	gameData->getRenderer().getWindow().setKeyRepeatEnabled(false);
}

void ph::EventLoop::eventLoop(GameData* gameData)
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

void ph::EventLoop::clear()
{
	mIsKeyJustPressed = false;
	mIsKeyJustReleased = false;
	mIsMouseButtonJustPressed = false;
	mIsMouseButtonJustReleased = false;
	mHasMouseJustMoved = false;
	mKey = sf::Keyboard::Unknown;
	mMouseButton = sf::Mouse::ButtonCount;
}
