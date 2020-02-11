#include "game.hpp"
#include "Events/globalKeyboardShortcuts.hpp"
#include "Events/eventDispatcher.hpp"
#include "Events/actionEventManager.hpp"
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

	Terminal::init(&mWindow, mSceneManager.get());
	mSceneManager->init(mTextures.get(), mAIManager.get());
	mSceneManager->replaceScene("scenes/mainMenu.xml");

	mWindow.setVerticalSyncEnabled(true);
	mWindow.setKeyRepeatEnabled(false);

	Widget::setWindow(&mWindow);
	XmlGuiParser::init(mTextures.get(), mSceneManager.get());

	ActionEventManager::init();
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
	ph::Event phEvent;
	while(EventDispatcher::dispatchEvent(phEvent, mWindow))
	{
		if (auto * event = std::get_if<sf::Event>(&phEvent))
			if (event->type == sf::Event::Closed)
				sIsRunning = false;

		handleGlobalKeyboardShortcuts(mWindow, phEvent);
		mFPSCounter.handleEvent(phEvent);
		Terminal::handleEvent(phEvent);
		GUI::handleEvent(phEvent);
		
		mSceneManager->handleEvent(phEvent);

		Renderer::handleEvent(phEvent);
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
