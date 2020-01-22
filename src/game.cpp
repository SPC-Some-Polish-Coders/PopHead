#include <GL/glew.h>
#include "game.hpp"
#include "Resources/loadFonts.hpp"
#include "Events/globalKeyboardShortcuts.hpp"
#include "Events/eventDispatcher.hpp"
#include "Events/actionEventManager.hpp"
#include "GUI/xmlGuiParser.hpp"
#include "Logs/logs.hpp"
#include "Renderer/renderer.hpp"
#include <SFML/System.hpp>

namespace ph {

Game::Game()
	:mWindow(sf::VideoMode::getDesktopMode(), "PopHead", sf::Style::Default, sf::ContextSettings(24, 8, 0, 3, 3))
	,mGameData()
	,mSoundPlayer(std::make_unique<SoundPlayer>())
	,mMusicPlayer(std::make_unique<MusicPlayer>())
	,mTextures(std::make_unique<TextureHolder>())
	,mFonts(std::make_unique<OldFontHolder>())
	,mAIManager(std::make_unique<AIManager>())
	,mSceneManager(std::make_unique<SceneManager>())
	,mTerminal(std::make_unique<Terminal>())
	,mDebugCounter(std::make_unique<FPSCounter>())
	,mGui(std::make_unique<GUI>())
{
	mGameData.reset(new GameData(
		&mWindow,
		mSoundPlayer.get(),
		mMusicPlayer.get(),
		mTextures.get(),
		mFonts.get(),
		mAIManager.get(),
		mSceneManager.get(),
		mTerminal.get(),
		mGui.get()
	));

	Renderer::init(sf::VideoMode::getDesktopMode().width, sf::VideoMode::getDesktopMode().height);
	
	GameData* gameData = mGameData.get();

	loadFonts(gameData);
	mTerminal->init(gameData);
	mSceneManager->setGameData(gameData);
	mSceneManager->replaceScene("scenes/mainMenu.xml");

	mWindow.setVerticalSyncEnabled(true);
	mWindow.setKeyRepeatEnabled(false);

	Widget::setWindow(&mWindow);
	XmlGuiParser::init(mGui.get(), mTextures.get(), mSceneManager.get(), &mGameData->getGameCloser(), mMusicPlayer.get(), mSoundPlayer.get());

	ActionEventManager::init();
}

void Game::run()
{
	sf::Clock clock;
	while(mGameData->getGameCloser().shouldGameBeClosed() == false)
	{
		mSceneManager->changingScenesProcess();
		handleEvents();
		const float dt = clock.restart().asSeconds();
		constexpr float minimalDTConstrain = 1.f/20.f;
		update(dt > minimalDTConstrain ? minimalDTConstrain : dt);
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
				mGameData->getGameCloser().closeGame();

		handleGlobalKeyboardShortcuts(mGameData->getWindow(), mGameData->getGameCloser(), phEvent);
		mDebugCounter->handleEvent(phEvent);
		mTerminal->handleEvent(phEvent);
		mGui->handleEvent(phEvent);
		
		if(!mTerminal->getSharedData()->isVisible)
			mSceneManager->handleEvent(phEvent);

		Renderer::handleEvent(phEvent);
	}
}

void Game::update(float dt)
{
	mDebugCounter->sampleFrame();

	if(mWindow.hasFocus())
	{
		mSceneManager->update(dt);
		mAIManager->update();
		mGui->update(dt);
		mTerminal->update();
		mDebugCounter->update();

		{
		static float width = 100.f;
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::T))
			width -= 5.f;
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Y))
			width += 5.f;
		Renderer::submitQuad(0, 0, &sf::Color::Red, 0, {100.f, 100.f}, {width, 4.f}, 0, 0.f, {}, ProjectionType::gui);
		Renderer::submitTextArea("abcdef 1234 lol", "joystixMonospace.ttf", {100.f, 100.f}, width, TextAligment::center, 30, sf::Color::Red, ProjectionType::gui);
		}

		Renderer::endScene(mWindow);
		mWindow.display();
	}
}

}
