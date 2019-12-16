#include <GL/glew.h>
#include "game.hpp"
#include "Resources/loadFonts.hpp"
#include "Events/globalKeyboardShortcuts.hpp"
#include "Events/eventDispatcher.hpp"
#include "Events/actionEventManager.hpp"
#include "Logs/logs.hpp"
#include "Renderer/renderer.hpp"
#include "Renderer/TemporaryTests/rendererTest.h"
#include <SFML/System.hpp>
#include <iostream>

namespace ph {

Game::Game()
	:mGameData{}
	,mWindow(sf::VideoMode::getDesktopMode(), "PopHead", sf::Style::Default,
		sf::ContextSettings(24, 8, 0, 3, 3/*, sf::ContextSettings::Core*/)
	)
	,mSoundPlayer{new SoundPlayer()}
	,mMusicPlayer{new MusicPlayer()}
	,mTextures{new TextureHolder()}
	,mFonts{new FontHolder()}
	,mShaders{new ShaderHolder()}
	,mAIManager(new AIManager())
	,mSceneManager{new SceneManager()}
	,mTerminal{new Terminal()}
	,mDebugCounter{new DebugCounter()}
	,mGui{new GUI()}
{
	mGameData.reset(new GameData(
		&mWindow,
		mSoundPlayer.get(),
		mMusicPlayer.get(),
		mTextures.get(),
		mFonts.get(),
		mShaders.get(),
		mAIManager.get(),
		mSceneManager.get(),
		nullptr,
		mTerminal.get(),
		mGui.get()
	));

	GameData* gameData = mGameData.get();

	loadFonts(gameData);
	mTerminal->init(gameData);
	mDebugCounter->init(*mFonts);
	mGui->init(gameData);
	mSceneManager->setGameData(gameData);
	mSceneManager->replaceScene("scenes/ecsTest.xml");

	mWindow.setVerticalSyncEnabled(true);

	ActionEventManager::init();

	Renderer::init(sf::VideoMode::getDesktopMode().width, sf::VideoMode::getDesktopMode().height);
}

void Game::run()
{
	sf::Clock clock;
	const sf::Time timePerFrame = sf::seconds(1.f / 60.f);
	sf::Time dt = sf::Time::Zero;

	while(mGameData->getGameCloser().shouldGameBeClosed() == false)
	{
		mSceneManager->changingScenesProcess();

		dt += clock.restart();

		while(dt >= timePerFrame) {
			handleEvents();
			update(corectDeltaTime(dt));
			dt = sf::Time::Zero;
		}
	}

	Renderer::shutDown();
	mWindow.close();
}

sf::Time Game::corectDeltaTime(sf::Time dt)
{
	const sf::Time dtMinimalConstrain = sf::seconds(1.f/20.f);
	return dt > dtMinimalConstrain ? dtMinimalConstrain : dt;
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
		
		if(!mTerminal->getSharedData()->mIsVisible)
			mSceneManager->handleEvent(phEvent);

		if(auto* sfEvent = std::get_if<sf::Event>(&phEvent))
			if(sfEvent->type == sf::Event::Resized)
				Renderer::onWindowResize(sfEvent->size.width, sfEvent->size.height);
	}
}

void Game::update(sf::Time dt)
{
	mDebugCounter->update();

	if(mWindow.hasFocus())
	{
		mSceneManager->update(dt);
		//mAIManager->update();
		//mPhysicsEngine->update(deltaTime);
		mGui->update(dt);
		mDebugCounter->draw();
		mTerminal->update();

		Renderer::endScene(mWindow, *mDebugCounter);
		mWindow.display();
	}
}

}
