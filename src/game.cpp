#include "game.hpp"
#include "GUI/xmlGuiParser.hpp"
#include "GUI/gui.hpp"
#include "Logs/logs.hpp"
#include "Terminal/terminal.hpp"
#include "Renderer/renderer.hpp"
#include "Audio/Sound/soundPlayer.hpp"
#include "Audio/Music/musicPlayer.hpp"
#include <SFML/System.hpp>

namespace ph {

Game::Game()
	:mWindow(sf::VideoMode::getDesktopMode(), "PopHead", sf::Style::Default, sf::ContextSettings(24, 8, 0, 3, 3, sf::ContextSettings::Core))
	,mTextures(std::make_unique<TextureHolder>())
	,mAIManager(std::make_unique<AIManager>())
	,mSceneManager(std::make_unique<SceneManager>())
{
	Renderer::init(sf::VideoMode::getDesktopMode().width, sf::VideoMode::getDesktopMode().height);
	SoundPlayer::init();
	MusicPlayer::init();

	mSceneManager->init(mTextures.get(), mAIManager.get());
	mSceneManager->replaceScene("scenes/mainMenu.xml");

	Terminal::init(&mWindow, mSceneManager.get());

	mWindow.setVerticalSyncEnabled(true);
	mWindow.setKeyRepeatEnabled(false);

	Widget::setWindow(&mWindow);
	XmlGuiParser::init(mTextures.get(), mSceneManager.get());
}

void Game::run()
{
	sf::Clock clock;
	while(sIsRunning)
	{
		mSceneManager->changingScenesProcess();
		handleEvents();
		const float dt = clock.restart().asSeconds();
		constexpr float maxDTConstrain = 1.f/20.f;
		update(dt > maxDTConstrain ? maxDTConstrain : dt);
	}

	Renderer::shutDown();
	mWindow.close();
}

void Game::handleEvents()
{
	sf::Event e;
	while(mWindow.pollEvent(e))
	{
		if(e.type == sf::Event::Closed)
		{
			sIsRunning = false;
		}
		if(e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::F11)
		{
			if(mWindow.getSize() == sf::Vector2u(sf::VideoMode::getDesktopMode().width, sf::VideoMode::getDesktopMode().height)) {
				mWindow.create(sf::VideoMode(640, 360), "PopHead", sf::Style::Default, sf::ContextSettings(24, 8, 0, 3, 3));
				Renderer::restart(640, 360);
				Widget::setScreenSize({640, 360});
			}
			else {
				mWindow.create(sf::VideoMode(), "PopHead", sf::Style::Fullscreen, sf::ContextSettings(24, 8, 0, 3, 3));
				sf::Vector2u fullScreenSize(sf::VideoMode::getDesktopMode().width, sf::VideoMode::getDesktopMode().height);
				Renderer::restart(fullScreenSize.x, fullScreenSize.y);
				Widget::setScreenSize(static_cast<sf::Vector2f>(fullScreenSize));
			}
			mWindow.setVerticalSyncEnabled(true);
			mWindow.setKeyRepeatEnabled(false);
		}

		mFPSCounter.handleEvent(e);
		Terminal::handleEvent(e);
		GUI::handleEvent(e);
		mSceneManager->handleEvent(e);
		Renderer::handleEvent(e);
	}
}

void Game::update(float dt)
{
	if(mWindow.hasFocus() || sNoFocusUpdate)
	{
		Renderer::beginScene();
		
		mSceneManager->update(dt);
		mAIManager->update();
		GUI::update(dt);
		Terminal::update(dt);
		mFPSCounter.update();

		Renderer::endScene();
		mWindow.display();
	}
}

}
